/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Broadcast Tests
*/

#include "connection/client.h"
#include "connection/server.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "map/resources.h"
#include "map/tile.h"
#include "team/player/player.h"
#include "vector.h"
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>

// Helper to create a test player at a given position and orientation
static player_t *create_test_player(
    game_t *game, pos_t pos, orientation_t orientation, int id)
{
    client_t *client = malloc(sizeof(client_t));
    client->sockfd = -1;
    client->server = game->server;
    client->is_gui = false;
    client->player = NULL;
    client->index = 0;
    client->command_buffer = vector_new(sizeof(void *));
    client->writing_buffer = vector_new(sizeof(char *));
    player_t *player = malloc(sizeof(player_t));
    player->id = id;
    player->pos = pos;
    player->orientation = orientation;
    player->level = 1;
    player->tick_cooldown = 0;
    player->hunger_cooldown = 126;
    player->doing_action = false;
    player->in_incantation = false;
    player->team = game->teams[0];
    player->client = client;
    // Add player to their team's players vector
    const vector_vtable_t *team_vtable = vector_get_vtable(player->team->players);
    team_vtable->push_back(player->team->players, &player);
    for (resource_t r = FOOD; r < RESOURCE_COUNT; r++)
        player->inventory[r] = 0;
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
        if (player->client->writing_buffer) {
            const vector_vtable_t *vtable =
                vector_get_vtable(player->client->writing_buffer);
            size_t sz = vtable->size(player->client->writing_buffer);
            for (size_t i = 0; i < sz; ++i) {
                char *msg =
                    *(char **)vtable->at(player->client->writing_buffer, i);
                free(msg);
            }
            vector_destroy(player->client->writing_buffer);
        }
        if (player->client->command_buffer)
            vector_destroy(player->client->command_buffer);
        free(player->client);
        player->client = NULL;
    }
    free(player);
}

static game_t *create_test_game(void)
{
    server_t *server = malloc(sizeof(server_t));
    server->options = malloc(sizeof(server_options_t));
    server->options->debug = false;
    server->options->port = 4242;
    server->options->width = 5;
    server->options->height = 5;
    server->options->frequency = 100;
    server->game = NULL;
    for (int i = 0; i < MAX_CLIENTS; i++)
        server->clients[i] = NULL;
    game_t *game = malloc(sizeof(game_t));
    game->server = server;
    game->map = create_map(5, 5, server);
    game->incantations = vector_new(sizeof(void *));
    game->next_player_id = 1;
    game->next_egg_id = 1;
    game->tick_rate = 100;
    game->game_tick = 0;
    game->teams = malloc(sizeof(team_t *) * 2);
    game->teams[0] = malloc(sizeof(team_t));
    game->teams[0]->name = strdup("team1");
    game->teams[0]->players = vector_new(sizeof(player_t *));
    game->teams[0]->eggs = vector_new(sizeof(void *));
    game->teams[0]->game = game;
    game->teams[1] = NULL;
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
    if (game->teams) {
        for (int i = 0; game->teams[i]; i++) {
            if (game->teams[i]->name)
                free(game->teams[i]->name);
            if (game->teams[i]->players)
                vector_destroy(game->teams[i]->players);
            if (game->teams[i]->eggs)
                vector_destroy(game->teams[i]->eggs);
            free(game->teams[i]);
        }
        free(game->teams);
    }
    if (game->server) {
        if (game->server->options)
            free(game->server->options);
        free(game->server);
    }
    free(game);
}

Test(broadcast, get_direction_and_broadcast_direction, .timeout = 2)
{
    game_t *game = create_test_game();
    pos_t p1 = {0, 0};
    pos_t p2 = {0, 1};
    pos_t p3 = {4, 4};
    player_t *north = create_test_player(game, p1, NORTH, 1);
    player_t *south = create_test_player(game, p2, SOUTH, 2);
    player_t *west = create_test_player(game, p3, WEST, 3);
    // Assert team vector is not empty
    const vector_vtable_t *team_vtable = vector_get_vtable(game->teams[0]->players);
    cr_assert(team_vtable->size(game->teams[0]->players) > 0, "Team should have players");
    // North at (0,0), South at (0,1), West at (4,4)
    direction_t d1 = get_broadcast_direction(north, south, game->map);
    direction_t d2 = get_broadcast_direction(south, north, game->map);
    direction_t d3 = get_broadcast_direction(west, north, game->map);
    cr_assert_neq(d1, DIRECTION_COUNT);
    cr_assert_neq(d2, DIRECTION_COUNT);
    cr_assert_neq(d3, DIRECTION_COUNT);
    destroy_test_player(north);
    destroy_test_player(south);
    destroy_test_player(west);
    destroy_test_game(game);
}

Test(broadcast, broadcast_to_all_players, .timeout = 2)
{
    game_t *game = create_test_game();
    pos_t p1 = {1, 1};
    pos_t p2 = {2, 2};
    pos_t p3 = {3, 3};
    player_t *sender = create_test_player(game, p1, NORTH, 1);
    player_t *receiver1 = create_test_player(game, p2, EAST, 2);
    player_t *receiver2 = create_test_player(game, p3, SOUTH, 3);
    // Debug: print team vector size and player addresses
    const vector_vtable_t *team_vtable = vector_get_vtable(game->teams[0]->players);
    size_t team_size = team_vtable->size(game->teams[0]->players);
    printf("Team has %zu players\n", team_size);
    for (size_t i = 0; i < team_size; ++i) {
        player_t *p = *(player_t **)team_vtable->at(game->teams[0]->players, i);
        if (p)
            printf("Player %zu at %p (id=%lu)\n", i, (void*)p, (unsigned long)p->id);
        else
            printf("Player %zu at %p (id=NULL)\n", i, (void*)p);
    }
    // Clear writing buffers before broadcast
    const vector_vtable_t *vtable1 =
        vector_get_vtable(receiver1->client->writing_buffer);
    const vector_vtable_t *vtable2 =
        vector_get_vtable(receiver2->client->writing_buffer);
    while (vtable1->size(receiver1->client->writing_buffer) > 0)
        vtable1->erase(receiver1->client->writing_buffer, 0);
    while (vtable2->size(receiver2->client->writing_buffer) > 0)
        vtable2->erase(receiver2->client->writing_buffer, 0);
    broadcast_to_all_players(game, sender, "hello world");
    // Check that both receivers got a message
    cr_assert(vtable1->size(receiver1->client->writing_buffer) > 0);
    cr_assert(vtable2->size(receiver2->client->writing_buffer) > 0);
    char *msg1 = *(char **)vtable1->at(receiver1->client->writing_buffer, 0);
    char *msg2 = *(char **)vtable2->at(receiver2->client->writing_buffer, 0);
    cr_assert(strstr(msg1, "hello world") != NULL);
    cr_assert(strstr(msg2, "hello world") != NULL);
    destroy_test_player(sender);
    destroy_test_player(receiver1);
    destroy_test_player(receiver2);
    destroy_test_game(game);
}

Test(broadcast, get_direction_cases, .timeout = 2)
{
    server_t *server = malloc(sizeof(server_t));
    server->options = malloc(sizeof(server_options_t));
    server->options->width = 5;
    server->options->height = 5;
    map_t *map = create_map(5, 5, server);
    // Same position
    pos_t a = {2, 2};
    pos_t b = {2, 2};
    cr_assert_eq(get_direction(a, NORTH, b, map), MIDDLE);
    // Directly in front (north)
    pos_t north = {2, 1};
    cr_assert_eq(get_direction(a, NORTH, north, map), FRONT);
    // Directly behind (south)
    pos_t south = {2, 3};
    cr_assert_eq(get_direction(a, NORTH, south, map), BACK);
    // Directly right (east)
    pos_t east = {3, 2};
    cr_assert_eq(get_direction(a, NORTH, east, map), RIGHT);
    // Directly left (west)
    pos_t west = {1, 2};
    cr_assert_eq(get_direction(a, NORTH, west, map), LEFT);
    // Diagonals
    pos_t ne = {3, 1};
    cr_assert_eq(get_direction(a, NORTH, ne, map), FRONT_RIGHT);
    pos_t nw = {1, 1};
    cr_assert_eq(get_direction(a, NORTH, nw, map), FRONT_LEFT);
    pos_t se = {3, 3};
    cr_assert_eq(get_direction(a, NORTH, se, map), BACK_RIGHT);
    pos_t sw = {1, 3};
    cr_assert_eq(get_direction(a, NORTH, sw, map), BACK_LEFT);
    // Test get_broadcast_direction for two players
    game_t *game = malloc(sizeof(game_t));
    game->map = map;
    team_t *team = malloc(sizeof(team_t));
    team->players = vector_new(sizeof(player_t *));
    team->eggs = vector_new(sizeof(void *));
    team->name = strdup("team1");
    team->game = game;
    game->teams = malloc(sizeof(team_t *) * 2);
    game->teams[0] = team;
    game->teams[1] = NULL;
    player_t *p1 = malloc(sizeof(player_t));
    player_t *p2 = malloc(sizeof(player_t));
    p1->pos = a; p1->orientation = NORTH; p1->team = team;
    p2->pos = north; p2->orientation = NORTH; p2->team = team;
    cr_assert_eq(get_broadcast_direction(p1, p2, map), FRONT);
    p2->pos = south;
    cr_assert_eq(get_broadcast_direction(p1, p2, map), BACK);
    p2->pos = east;
    cr_assert_eq(get_broadcast_direction(p1, p2, map), RIGHT);
    p2->pos = west;
    cr_assert_eq(get_broadcast_direction(p1, p2, map), LEFT);
    // Clean up
    free(p1);
    free(p2);
    vector_destroy(team->players);
    vector_destroy(team->eggs);
    free(team->name);
    free(team);
    free(game->teams);
    free(game);
    destroy_map(map);
    free(server->options);
    free(server);
}
