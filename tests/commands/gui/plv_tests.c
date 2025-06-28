/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** plv Event/Command Tests
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "command_handler/gui_commands.h"
#include "constants.h"
#include "options_parser/options.h"
#include "team/player/player.h"
#include "game/game.h"
#include "vector.h"

static server_t *create_test_server(void) { server_t *server = malloc(sizeof(server_t)); if (!server) return NULL; server->options = malloc(sizeof(server_options_t)); if (!server->options) { free(server); return NULL; } server->options->debug = false; server->options->port = 4242; server->options->width = 10; server->options->height = 10; server->options->frequency = 100; server->game = NULL; for (int i = 0; i < MAX_CLIENTS; i++) server->clients[i] = NULL; return server; }
static void destroy_test_server(server_t *server) { if (!server) return; if (server->options) free(server->options); free(server); }
static client_t *create_test_client(server_t *server) { client_t *client = malloc(sizeof(client_t)); if (!client) return NULL; client->sockfd = -1; client->server = server; client->is_gui = true; client->player = NULL; client->index = 0; client->command_buffer = vector_new(sizeof(command_t *)); client->writing_buffer = vector_new(sizeof(char *)); return client; }
static void destroy_test_client(client_t *client) { if (!client) return; if (client->writing_buffer) { clear_writing_buffer(client); vector_destroy(client->writing_buffer); client->writing_buffer = NULL; } if (client->command_buffer) { vector_destroy(client->command_buffer); client->command_buffer = NULL; } free(client); }
static int create_test_pipe(int *write_fd) { int pipefd[2]; if (pipe(pipefd) == -1) return -1; *write_fd = pipefd[1]; return pipefd[0]; }
static char *read_from_pipe(int read_fd) { char buffer[256]; ssize_t bytes_read; bytes_read = read(read_fd, buffer, sizeof(buffer) - 1); if (bytes_read <= 0) return NULL; buffer[bytes_read] = '\0'; return strdup(buffer); }
static void flush_client_writing_buffer(client_t *client) { server_t temp_server; temp_server.clients[0] = client; for (int i = 1; i < MAX_CLIENTS; i++) temp_server.clients[i] = NULL; handle_clients_writing_buffer(&temp_server); }

Test(plv_event, valid_player, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    server->clients[0] = client;
    player_t *player = malloc(sizeof(player_t));
    cr_assert_not_null(player, "Player allocation failed");
    player->id = 77;
    player->level = 3;
    player->client = client;
    plv_event(player);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "plv #77 3") != NULL, "Should output correct plv event");
    close(read_fd);
    free(response);
    free(player);
    destroy_test_client(client);
    destroy_test_server(server);
}

Test(plv_event, null_player, .timeout = 2) {
    plv_event(NULL);
}

Test(plv_command, valid_player, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    server->clients[0] = client;
    game_t *game = malloc(sizeof(game_t));
    cr_assert_not_null(game, "Game allocation failed");
    team_t *team = malloc(sizeof(team_t));
    cr_assert_not_null(team, "Team allocation failed");
    player_t *player = malloc(sizeof(player_t));
    cr_assert_not_null(player, "Player allocation failed");
    player->id = 42;
    player->level = 7;
    player->client = client;
    team->players = vector_new(sizeof(player_t *));
    team->name = strdup("test");
    team->eggs = vector_new(sizeof(void *));
    team->game = game;
    team_t *teams[2] = {team, NULL};
    game->teams = teams;
    game->server = server;
    add_player_to_team(team, player);
    server->game = game;
    client->server = server;
    command_t cmd = { .argc = 1, .argv = (char *[1]){"42"} };
    plv_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "plv #42 7") != NULL, "Should output correct plv command");
    close(read_fd);
    vector_destroy(team->players);
    vector_destroy(team->eggs);
    free(team->name);
    free(team);
    free(game);
    free(player);
    destroy_test_client(client);
    destroy_test_server(server);
}

Test(plv_command, invalid_argc, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    client->sockfd = write_fd;
    server->clients[0] = client;
    command_t cmd = { .argc = 0, .argv = NULL };
    plv_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "sbp") != NULL, "Should output sbp for invalid argc");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_server(server);
}

Test(plv_command, invalid_player_id, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    client->sockfd = write_fd;
    server->clients[0] = client;
    command_t cmd = { .argc = 1, .argv = (char *[1]){"0"} };
    plv_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "sbp") != NULL, "Should output sbp for invalid player id");
    close(read_fd);
    free(response);
    destroy_test_client(client);
    destroy_test_server(server);
}

Test(plv_command, player_not_found, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    client->sockfd = write_fd;
    server->clients[0] = client;
    game_t *game = malloc(sizeof(game_t));
    cr_assert_not_null(game, "Game allocation failed");
    team_t *team = malloc(sizeof(team_t));
    cr_assert_not_null(team, "Team allocation failed");
    team->players = vector_new(sizeof(player_t *));
    team->name = strdup("test");
    team->eggs = vector_new(sizeof(void *));
    team->game = game;
    team_t *teams[2] = {team, NULL};
    game->teams = teams;
    game->server = server;
    server->game = game;
    command_t cmd = { .argc = 1, .argv = (char *[1]){"999"} };
    plv_command(client, &cmd);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "sbp") != NULL, "Should output sbp for player not found");
    close(read_fd);
    vector_destroy(team->players);
    vector_destroy(team->eggs);
    free(team->name);
    free(team);
    free(game);
    destroy_test_client(client);
    destroy_test_server(server);
}
