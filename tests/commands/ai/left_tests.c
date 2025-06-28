/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Left Command Tests
*/

#include <criterion/criterion.h>
#include <stdlib.h>
#include "map/coordinates.h"
#include "options_parser/options.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "game/game.h"
#include "team/player/player.h"
#include "map/map.h"
#include "game/game_constants.h"
#include "command_handler/ai_commands.h"
#include "connection/writing_buffer.h"
#include "constants.h"
#include "game/incantation.h"
#include "map/resources.h"
#include "map/tile.h"
#include "vector.h"
#include <string.h>

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
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i] = NULL;
    }
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
    client->is_gui = false;
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

static game_t *create_test_game(void)
{
    server_t *server = create_test_server();
    if (!server)
        return NULL;
    game_t *game = malloc(sizeof(game_t));
    if (!game) {
        destroy_test_server(server);
        return NULL;
    }
    game->server = server;
    game->map = create_map(10, 10, server);
    game->incantations = vector_new(sizeof(incantation_t *));
    game->next_player_id = 1;
    game->next_egg_id = 1;
    game->tick_rate = 100;
    game->game_tick = 0;
    game->teams = malloc(sizeof(team_t *) * 3);
    game->teams[0] = create_test_team("team1", game);
    game->teams[1] = create_test_team("team2", game);
    game->teams[2] = NULL;
    server->game = game;
    return game;
}

static void destroy_test_game(game_t *game)
{
    if (!game)
        return;
    if (game->incantations) {
        vector_destroy(game->incantations);
    }
    if (game->map)
        destroy_map(game->map);
    if (game->teams) {
        for (int i = 0; game->teams[i]; i++) {
            destroy_test_team(game->teams[i]);
        }
        free(game->teams);
    }
    if (game->server)
        destroy_test_server(game->server);
    free(game);
}

static player_t *create_test_player(game_t *game, pos_t pos, level_t level)
{
    client_t *client = create_test_client(game->server);
    if (!client)
        return NULL;
    player_t *player = malloc(sizeof(player_t));
    if (!player) {
        destroy_test_client(client);
        return NULL;
    }
    player->id = game->next_player_id++;
    player->pos = pos;
    player->orientation = NORTH;
    player->level = level;
    player->tick_cooldown = 0;
    player->hunger_cooldown = 126;
    player->doing_action = false;
    player->in_incantation = false;
    player->team = game->teams[0];
    player->client = client;
    for (resource_t r = FOOD; r < RESOURCE_COUNT; r++) {
        player->inventory[r] = 0;
    }
    player->inventory[FOOD] = 10;
    client->player = player;
    tile_t *tile = get_tile(game->map, pos);
    if (tile) {
        const vector_vtable_t *vtable = vector_get_vtable(tile->players);
        vtable->push_back(tile->players, &player);
    }
    return player;
}

static void destroy_test_player(player_t *player)
{
    if (!player)
        return;
    if (player->client) {
        destroy_test_client(player->client);
        player->client = NULL;
    }
    free(player);
}

Test(left_command, turn_left, .timeout = 2) {
    server_t *server = create_test_server();
    server->game = create_test_game();
    game_t *game = server->game;
    pos_t pos = {2, 2};
    client_t *client = create_test_client(server);
    player_t *player = create_test_player(game, pos, 1);
    client->player = player;
    command_t cmd = {0};
    cmd.name = strdup("Left");
    cmd.argc = 0;
    cmd.argv = NULL;
    left_command(client, &cmd);
    cr_assert_eq(client->player->orientation, WEST);
    left_command(client, &cmd);
    cr_assert_eq(client->player->orientation, SOUTH);
    left_command(client, &cmd);
    cr_assert_eq(client->player->orientation, EAST);
    left_command(client, &cmd);
    cr_assert_eq(client->player->orientation, NORTH);
    free(cmd.name);
    destroy_test_player(player);
    destroy_test_client(client);
    destroy_test_game(game);
    destroy_test_server(server);
}
