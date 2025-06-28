/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** pic Event Tests
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "command_handler/gui_commands.h"
#include "constants.h"
#include "game/incantation.h"
#include "map/coordinates.h"
#include "options_parser/options.h"
#include "team/player/player.h"
#include "vector.h"

// --- Mock helpers (consistent with other GUI event tests) ---

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
    client->command_buffer = vector_new(sizeof(void *));
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

static int create_test_pipe(int *write_fd)
{
    int pipefd[2];
    if (pipe(pipefd) == -1) return -1;
    *write_fd = pipefd[1];
    return pipefd[0];
}

static char *read_from_pipe(int read_fd)
{
    char buffer[1024];
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

// --- pic_event tests ---

Test(pic_event, valid_incantation_multiple_players, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    server->clients[0] = client;
    player_t *p1 = malloc(sizeof(player_t));
    player_t *p2 = malloc(sizeof(player_t));
    cr_assert_not_null(p1);
    cr_assert_not_null(p2);
    p1->id = 1; p1->client = client;
    p2->id = 2; p2->client = client;
    incantation_t incantation = {0};
    incantation.pos.x = 3;
    incantation.pos.y = 4;
    incantation.level = 5;
    player_t *players[] = {p1, p2, NULL};
    incantation.players = players;
    pic_event(&incantation, server);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "pic 3 4 5 1 2") != NULL, "Should output correct pic event with both player IDs");
    close(read_fd);
    free(response);
    free(p1);
    free(p2);
    destroy_test_client(client);
    destroy_test_server(server);
}

Test(pic_event, valid_incantation_single_player, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    server->clients[0] = client;
    player_t *p1 = malloc(sizeof(player_t));
    cr_assert_not_null(p1);
    p1->id = 42; p1->client = client;
    incantation_t incantation = {0};
    incantation.pos.x = 1;
    incantation.pos.y = 2;
    incantation.level = 3;
    player_t *players[] = {p1, NULL};
    incantation.players = players;
    pic_event(&incantation, server);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "pic 1 2 3 42") != NULL, "Should output correct pic event with single player ID");
    close(read_fd);
    free(response);
    free(p1);
    destroy_test_client(client);
    destroy_test_server(server);
}

Test(pic_event, null_incantation, .timeout = 2) {
    server_t *server = create_test_server();
    // Should not crash or output anything
    pic_event(NULL, server);
    cr_assert(1, "pic_event(NULL, ...) should not crash");
    destroy_test_server(server);
}

Test(pic_event, null_server, .timeout = 2) {
    incantation_t incantation = {0};
    // Should not crash or output anything
    pic_event(&incantation, NULL);
    cr_assert(1, "pic_event(..., NULL) should not crash");
}

Test(pic_event, player_null_or_no_client, .timeout = 2) {
    server_t *server = create_test_server();
    client_t *client = create_test_client(server);
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    server->clients[0] = client;
    player_t *p1 = malloc(sizeof(player_t));
    player_t *p2 = malloc(sizeof(player_t));
    cr_assert_not_null(p1);
    cr_assert_not_null(p2);
    p1->id = 1; p1->client = client;
    p2->id = 2; p2->client = NULL; // No client
    incantation_t incantation = {0};
    incantation.pos.x = 7;
    incantation.pos.y = 8;
    incantation.level = 9;
    player_t *players[] = {p1, NULL, p2, NULL};
    incantation.players = players;
    pic_event(&incantation, server);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "pic 7 8 9 1") != NULL, "Should output only valid player IDs");
    cr_assert(strstr(response, "2") == NULL, "Should not output player with no client");
    close(read_fd);
    free(response);
    free(p1);
    free(p2);
    destroy_test_client(client);
    destroy_test_server(server);
}
