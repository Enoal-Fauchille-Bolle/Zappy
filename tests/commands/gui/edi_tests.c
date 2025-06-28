/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** edi Command Tests
*/

#include "command_handler/command.h"
#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "constants.h"
#include "game/game.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "options_parser/options.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "vector.h"
#include <bits/types/struct_timeval.h>
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>

// --- Mock helpers (same structure as AI command tests) ---

static server_t *create_test_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server)
        return NULL;
    server->options = malloc(sizeof(server_options_t));
    if (!server->options) {
        free(server);
        return NULL;
    }
    server->options->debug = false;
    server->options->port = 4242;
    server->options->width = 10;
    server->options->height = 10;
    server->options->frequency = 100;
    server->game = NULL;
    for (int i = 0; i < MAX_CLIENTS; i++)
        server->clients[i] = NULL;
    return server;
}

static void destroy_test_server(server_t *server)
{
    if (!server)
        return;
    if (server->options)
        free(server->options);
    free(server);
}

static client_t *create_test_client(server_t *server)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client)
        return NULL;
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
    if (!client)
        return;
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
    if (!game)
        return NULL;
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
    if (!game)
        return;
    if (game->incantations)
        vector_destroy(game->incantations);
    if (game->map)
        destroy_map(game->map);
    if (game->server)
        destroy_test_server(game->server);
    free(game);
}

static egg_t *create_test_egg(int egg_id, pos_t pos)
{
    egg_t *egg = malloc(sizeof(egg_t));
    if (!egg)
        return NULL;
    egg->id = egg_id;
    egg->pos = pos;
    egg->team = NULL;
    return egg;
}

static void destroy_test_egg(egg_t *egg)
{
    if (egg)
        free(egg);
}

static team_t *create_test_team(const char *name, game_t *game)
{
    team_t *team = malloc(sizeof(team_t));
    if (!team)
        return NULL;
    team->name = strdup(name);
    team->players = vector_new(sizeof(player_t *));
    team->eggs = vector_new(sizeof(egg_t *));
    team->game = game;
    return team;
}

static void destroy_test_team(team_t *team)
{
    if (!team)
        return;
    if (team->name)
        free(team->name);
    if (team->players)
        vector_destroy(team->players);
    if (team->eggs)
        vector_destroy(team->eggs);
    free(team);
}

// --- Test helpers for socket output capture ---

static int create_test_pipe(int *write_fd)
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
        return -1;
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
    if (result <= 0)
        return NULL;
    bytes_read = read(read_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
        return NULL;
    buffer[bytes_read] = '\0';
    return strdup(buffer);
}

static void flush_client_writing_buffer(client_t *client)
{
    server_t temp_server;
    temp_server.clients[0] = client;
    for (int i = 1; i < MAX_CLIENTS; i++)
        temp_server.clients[i] = NULL;
    handle_clients_writing_buffer(&temp_server);
}

// --- edi_event tests ---

Test(edi_event, valid_egg, .timeout = 2)
{
    server_t *server = create_test_server();
    game_t *game = create_test_game(server);
    team_t *team = create_test_team("team1", game);
    pos_t pos = {3, 4};
    int egg_id = 42;
    egg_t *egg = create_test_egg(egg_id, pos);
    egg->team = team;
    // Register a GUI client to the server
    client_t *client = create_test_client(server);
    server->clients[0] = client;
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;
    edi_event(egg);
    flush_client_writing_buffer(client);
    close(write_fd);
    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert(strstr(response, "edi #42") != NULL,
        "Should output 'edi #42' for valid egg");
    close(read_fd);
    free(response);
    destroy_test_egg(egg);
    destroy_test_team(team);
    destroy_test_client(client);
    destroy_test_game(game);
}

Test(edi_event, null_egg, .timeout = 2)
{
    edi_event(NULL);
}
