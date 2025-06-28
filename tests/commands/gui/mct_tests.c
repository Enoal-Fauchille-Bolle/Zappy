/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** mct Command Tests
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
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
#include "map/map.h"
#include "map/tile.h"
#include "options_parser/options.h"
#include "vector.h"


static server_t *create_test_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server) return NULL;
    server->options = malloc(sizeof(server_options_t));
    if (!server->options) { free(server); return NULL; }
    server->options->debug = false;
    server->options->port = 4242;
    server->options->width = 3;
    server->options->height = 2;
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
    game->map = create_map(server->options->width, server->options->height, server);
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

static int create_test_pipe(int *write_fd)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    *write_fd = pipefd[1];
    return pipefd[0];
}

static char *read_from_pipe(int read_fd)
{
    char buffer[2048];
    ssize_t bytes_read;
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

// --- mct_command tests ---

Test(mct_command, outputs_all_tiles, .timeout = 2) {
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    command_t cmd = { .name = "mct", .argc = 0, .argv = NULL };
    mct_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    // Should output bct for every tile (width*height = 6)
    int count = 0;
    char *ptr = response;
    while ((ptr = strstr(ptr, "bct ")) != NULL) {
        count++;
        ptr += 4;
    }
    cr_assert_eq(count, 6, "Should output 6 bct lines for a 3x2 map");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_game(game);
}
