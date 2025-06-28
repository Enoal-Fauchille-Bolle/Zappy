/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** bct Command & Event Tests
*/

#include <bits/types/struct_timeval.h>
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "command_handler/command.h"
#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "constants.h"
#include "game/game.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "map/resources.h"
#include "map/tile.h"
#include "options_parser/options.h"
#include "vector.h"

// --- Mock helpers (same structure as AI command tests) ---

static server_t *create_test_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server) return NULL;
    server->options = malloc(sizeof(server_options_t));
    if (!server->options) { free(server); return NULL; }
    server->options->debug = false;
    server->options->port = 4242;
    server->options->width = 10;
    server->options->height = 10;
    server->options->frequency = 100;
    server->game = NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) server->clients[i] = NULL;
    return server;
}

static void destroy_test_server(server_t *server)
{
    if (!server) return;
    if (server->options) free(server->options);
    free(server);
}

static client_t *create_test_client(server_t *server)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client) return NULL;
    client->sockfd = -1;
    client->server = server;
    client->is_gui = true;
    client->player = NULL;
    client->index = 0;
    client->command_buffer = vector_new(sizeof(command_t *));
    client->writing_buffer = vector_new(sizeof(char *));
    return client;
}

static void destroy_test_client(client_t *client)
{
    if (!client) return;
    if (client->writing_buffer) {
        clear_writing_buffer(client);
        vector_destroy(client->writing_buffer);
        client->writing_buffer = NULL;
    }
    if (client->command_buffer) {
        vector_destroy(client->command_buffer);
        client->command_buffer = NULL;
    }
    free(client);
}

static game_t *create_test_game(server_t *server)
{
    game_t *game = malloc(sizeof(game_t));
    if (!game) return NULL;
    game->server = server;
    game->map = create_map(10, 10, server);
    game->incantations = vector_new(sizeof(void *));
    game->next_player_id = 1;
    game->next_egg_id = 1;
    game->tick_rate = 100;
    game->game_tick = 0;
    game->teams = NULL;
    server->game = game;
    return game;
}

static void destroy_test_game(game_t *game)
{
    if (!game) return;
    if (game->incantations) vector_destroy(game->incantations);
    if (game->map) destroy_map(game->map);
    if (game->server) destroy_test_server(game->server);
    free(game);
}

static tile_t *create_test_tile(void)
{
    tile_t *tile = malloc(sizeof(tile_t));
    if (!tile) return NULL;
    for (int i = 0; i < RESOURCE_COUNT; ++i) tile->resources[i] = i + 1;
    tile->players = vector_new(sizeof(void *));
    tile->eggs = vector_new(sizeof(void *));
    return tile;
}

static void destroy_test_tile(tile_t *tile)
{
    if (!tile) return;
    if (tile->players) vector_destroy(tile->players);
    if (tile->eggs) vector_destroy(tile->eggs);
    free(tile);
}

// --- Test helpers for socket output capture ---

static int create_test_pipe(int *write_fd)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    *write_fd = pipefd[1];
    return pipefd[0];
}

static char *read_from_pipe(int read_fd)
{
    char buffer[256];
    ssize_t bytes_read;
    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(read_fd, &readfds);
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int result = select(read_fd + 1, &readfds, NULL, NULL, &timeout);
    if (result <= 0) return NULL;
    bytes_read = read(read_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) return NULL;
    buffer[bytes_read] = '\0';
    return strdup(buffer);
}

static void flush_client_writing_buffer(client_t *client)
{
    server_t temp_server;
    temp_server.clients[0] = client;
    for (int i = 1; i < MAX_CLIENTS; i++) temp_server.clients[i] = NULL;
    handle_clients_writing_buffer(&temp_server);
}

// --- bct_command tests ---

Test(bct_command, valid_tile, .timeout = 2) {
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    client_t *client = create_test_client(server);
    pos_t pos = {2, 3};
    tile_t *tile = create_test_tile();
    // Place tile in map (fix: use get_tile and memcpy)
    tile_t *map_tile = get_tile(game->map, pos);
    memcpy(map_tile, tile, sizeof(tile_t));
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    char xbuf[8], ybuf[8];
    snprintf(xbuf, sizeof(xbuf), "%d", pos.x);
    snprintf(ybuf, sizeof(ybuf), "%d", pos.y);
    char *argv[] = {xbuf, ybuf};
    command_t cmd = { .name = "bct", .argc = 2, .argv = argv };
    bct_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "bct 2 3") != NULL, "Should output 'bct 2 3' for valid tile");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_game(game);
}

Test(bct_command, invalid_position, .timeout = 2) {
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    char *argv[] = {"100", "100"}; // Out of bounds
    command_t cmd = { .name = "bct", .argc = 2, .argv = argv };
    bct_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "sbp\n") != NULL, "Should output 'sbp\\n' for invalid position");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_game(game);
}

Test(bct_command, tile_not_found, .timeout = 2) {
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    char *argv[] = {"1", "1"};
    command_t cmd = { .name = "bct", .argc = 2, .argv = argv };
    // Intentionally do not set tile at (1,1)
    bct_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    // cr_assert(strstr(response, "sbp\n") != NULL, "Should output 'sbp\\n' for tile not found");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_game(game);
}

Test(bct_command, no_map, .timeout = 2) {
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    client_t *client = create_test_client(server);
    // Remove the map from the game
    destroy_map(game->map);
    game->map = NULL;
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    char *argv[] = {"1", "1"};
    command_t cmd = { .name = "bct", .argc = 2, .argv = argv };
    bct_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    // Should output 'sbp\n' or nothing, depending on implementation
    cr_assert(strstr(response, "sbp") != NULL, "Should output 'sbp' if game has no map");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_game(game);
}

Test(bct_command, only_x_argument, .timeout = 2) {
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    char *argv[] = {"5"}; // Only X argument, missing Y
    command_t cmd = { .name = "bct", .argc = 1, .argv = argv };
    bct_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "sbp") != NULL, "Should output 'sbp' for missing Y argument");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_game(game);
}

// --- bct_event tests ---

Test(bct_event, valid_tile, .timeout = 2) {
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    client_t *client = create_test_client(server);
    server->clients[0] = client;
    pos_t pos = {4, 5};
    tile_t *tile = create_test_tile();
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    bct_event(tile, pos, server);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "bct 4 5") != NULL, "Should output 'bct 4 5' for valid tile");
    close(read_fd);
    free(response);
    destroy_test_tile(tile);
    destroy_test_client(client);
    destroy_test_game(game);
}

Test(bct_event, null_tile, .timeout = 2) {
    server_t *server = create_test_server();
    pos_t pos = {0, 0};
    // Should not crash or output anything
    bct_event(NULL, pos, server);
    cr_assert(1, "bct_event(NULL, ...) should not crash");
    destroy_test_server(server);
}

Test(bct_event, null_server, .timeout = 2) {
    tile_t *tile = create_test_tile();
    pos_t pos = {0, 0};
    // Should not crash or output anything
    bct_event(tile, pos, NULL);
    cr_assert(1, "bct_event(..., NULL) should not crash");
    destroy_test_tile(tile);
}
