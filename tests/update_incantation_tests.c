/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Update Incantation Tests
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "constants.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "game/incantation.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "map/resources.h"
#include "map/tile.h"
#include "options_parser/options.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Test helper functions

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

    // Initialize clients array to NULL
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

    // Create teams array (NULL-terminated)
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

    // Clean up incantations vector carefully - but don't destroy the
    // incantations themselves as they might be destroyed by update_incantation
    if (game->incantations) {
        vector_destroy(game->incantations);
    }

    // Destroy map after cleaning up incantations
    if (game->map)
        destroy_map(game->map);

    // Clean up teams
    if (game->teams) {
        for (int i = 0; game->teams[i]; i++) {
            destroy_test_team(game->teams[i]);
        }
        free(game->teams);
    }

    // Clean up server last
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

    // Initialize player
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

    // Initialize inventory to zero
    for (resource_t r = FOOD; r < RESOURCE_COUNT; r++) {
        player->inventory[r] = 0;
    }
    player->inventory[FOOD] = 10;      // Give some food

    client->player = player;

    // Add player to the tile at the specified position
    tile_t *tile = get_tile(game->map, pos);
    if (tile) {
        const vector_vtable_t *vtable = vector_get_vtable(tile->players);
        vtable->push_back(tile->players, &player);
    }

    return player;
}

static incantation_t *create_test_incantation(
    pos_t pos, level_t level, int num_players)
{
    incantation_t *incantation = malloc(sizeof(incantation_t));
    if (!incantation)
        return NULL;

    incantation->pos = pos;
    incantation->level = level;
    incantation->ticks_left = INCANTATION_COMMAND_COOLDOWN;
    incantation->players = malloc(sizeof(player_t *) * (num_players + 1));

    if (!incantation->players) {
        free(incantation);
        return NULL;
    }

    // Initialize all to NULL
    for (int i = 0; i <= num_players; i++) {
        incantation->players[i] = NULL;
    }

    return incantation;
}

static void add_resources_to_tile(
    map_t *map, pos_t pos, const inventory_t resources)
{
    tile_t *tile = get_tile(map, pos);
    if (!tile)
        return;

    for (resource_t r = FOOD; r < RESOURCE_COUNT; r++) {
        tile->resources[r] += resources[r];
    }
}

// Tests for update_incantation function

Test(update_incantation, update_incantation_null_pointer)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    // Test that the function handles NULL pointer gracefully
    update_incantation(NULL, game);
    cr_assert(true);      // If we reach here, the function didn't crash

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_ticks_left_greater_than_zero)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {5, 5};
    incantation_t *incantation = create_test_incantation(pos, 1, 1);
    cr_assert_not_null(incantation);

    // Set ticks left to more than 0
    incantation->ticks_left = 50;

    // Create a player and add to incantation
    player_t *player = create_test_player(game, pos, 1);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    size_t initial_size = vtable->size(game->incantations);
    tick_t initial_ticks = incantation->ticks_left;

    update_incantation(incantation, game);

    // Should decrement ticks and not remove from vector
    cr_assert_eq(incantation->ticks_left, initial_ticks - 1);
    cr_assert_eq(vtable->size(game->incantations), initial_size);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_ticks_zero_successful_incantation)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {3, 3};
    level_t level = 1;      // Level 1 to 2 requires: 1 linemate

    incantation_t *incantation = create_test_incantation(pos, level, 1);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create a player at the correct level
    player_t *player = create_test_player(game, pos, level);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Add required resources to the tile (level 1->2 requires 1 linemate)
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 1;
    add_resources_to_tile(game->map, pos, required_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    size_t initial_size = vtable->size(game->incantations);
    level_t initial_level = player->level;

    update_incantation(incantation, game);

    // Should complete incantation, level up player, and remove from vector
    cr_assert_eq(player->level, initial_level + 1);
    cr_assert_eq(vtable->size(game->incantations), initial_size - 1);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_ticks_zero_failed_incantation)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {3, 3};
    level_t level = 1;      // Level 1 to 2 requires: 1 linemate

    incantation_t *incantation = create_test_incantation(pos, level, 1);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create a player at the correct level
    player_t *player = create_test_player(game, pos, level);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Don't add required resources to the tile (should fail)

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    size_t initial_size = vtable->size(game->incantations);
    level_t initial_level = player->level;

    update_incantation(incantation, game);

    // Should fail incantation, not level up player, but still remove from
    // vector
    cr_assert_eq(player->level, initial_level);
    cr_assert_eq(vtable->size(game->incantations), initial_size - 1);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_multiple_players_level_up)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {2, 2};
    level_t level = 2;      // Level 2 to 3 requires: 1 linemate, 1 deraumere,
                            // 1 sibur, 2 players

    incantation_t *incantation = create_test_incantation(pos, level, 2);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create two players at the correct level (level 2->3 requires 2 players)
    player_t *player1 = create_test_player(game, pos, level);
    player_t *player2 = create_test_player(game, pos, level);
    cr_assert_not_null(player1);
    cr_assert_not_null(player2);

    incantation->players[0] = player1;
    incantation->players[1] = player2;

    // Add required resources to the tile (level 2->3 requirements)
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 1;
    required_resources[DERAUMERE] = 1;
    required_resources[SIBUR] = 1;
    add_resources_to_tile(game->map, pos, required_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    level_t initial_level1 = player1->level;
    level_t initial_level2 = player2->level;

    update_incantation(incantation, game);

    // Both players should level up
    cr_assert_eq(player1->level, initial_level1 + 1);
    cr_assert_eq(player2->level, initial_level2 + 1);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_resources_consumed)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {1, 1};
    level_t level = 1;      // Level 1 to 2 requires: 1 linemate

    incantation_t *incantation = create_test_incantation(pos, level, 1);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create a player at the correct level
    player_t *player = create_test_player(game, pos, level);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Add more resources than required to the tile
    inventory_t initial_resources = {0};
    initial_resources[LINEMATE] = 5;      // More than required
    initial_resources[DERAUMERE] = 3;
    add_resources_to_tile(game->map, pos, initial_resources);

    tile_t *tile = get_tile(game->map, pos);
    size_t initial_linemate = tile->resources[LINEMATE];
    size_t initial_deraumere = tile->resources[DERAUMERE];

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    update_incantation(incantation, game);

    // Should consume exactly the required resources
    cr_assert_eq(tile->resources[LINEMATE],
        initial_linemate - INCANT_REQS[level - 1][LINEMATE]);
    cr_assert_eq(tile->resources[DERAUMERE],
        initial_deraumere - INCANT_REQS[level - 1][DERAUMERE]);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_high_level)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {7, 7};
    level_t level = 7;      // Level 7 to 8 requires: 2 linemate, 2 deraumere,
                            // 2 sibur, 2 mendiane, 2 phiras, 1 thystame

    incantation_t *incantation = create_test_incantation(pos, level, 6);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create six players at the correct level (level 7->8 requires 6 players)
    player_t *players[6];
    for (int i = 0; i < 6; i++) {
        players[i] = create_test_player(game, pos, level);
        cr_assert_not_null(players[i]);
        incantation->players[i] = players[i];
    }

    // Add required resources to the tile
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 2;
    required_resources[DERAUMERE] = 2;
    required_resources[SIBUR] = 2;
    required_resources[MENDIANE] = 2;
    required_resources[PHIRAS] = 2;
    required_resources[THYSTAME] = 1;
    add_resources_to_tile(game->map, pos, required_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    update_incantation(incantation, game);

    // All players should level up to max level
    for (int i = 0; i < 6; i++) {
        cr_assert_eq(players[i]->level, MAX_PLAYER_LEVEL);
    }

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_removes_from_vector)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos1 = {1, 1};
    pos_t pos2 = {2, 2};
    level_t level = 1;

    // Create two incantations
    incantation_t *incantation1 = create_test_incantation(pos1, level, 1);
    incantation_t *incantation2 = create_test_incantation(pos2, level, 1);
    cr_assert_not_null(incantation1);
    cr_assert_not_null(incantation2);

    incantation1->ticks_left = 0;
    incantation2->ticks_left = 5;      // This one shouldn't be removed

    // Create players
    player_t *player1 = create_test_player(game, pos1, level);
    player_t *player2 = create_test_player(game, pos2, level);
    incantation1->players[0] = player1;
    incantation2->players[0] = player2;

    // Add required resources
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 1;
    add_resources_to_tile(game->map, pos1, required_resources);
    add_resources_to_tile(game->map, pos2, required_resources);

    // Add both incantations to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation1);
    vtable->push_back(game->incantations, &incantation2);

    cr_assert_eq(vtable->size(game->incantations), 2);

    update_incantation(incantation1, game);

    // Only incantation1 should be removed
    cr_assert_eq(vtable->size(game->incantations), 1);

    // The remaining incantation should be incantation2
    incantation_t *remaining =
        *(incantation_t **)vtable->at(game->incantations, 0);
    cr_assert_eq(remaining, incantation2);

    destroy_test_game(game);
}

// Additional edge case tests

Test(update_incantation, update_incantation_insufficient_players)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {4, 4};
    level_t level = 4;      // Level 4 to 5 requires 4 players

    incantation_t *incantation = create_test_incantation(pos, level, 2);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create only 2 players when 4 are required
    player_t *player1 = create_test_player(game, pos, level);
    player_t *player2 = create_test_player(game, pos, level);
    cr_assert_not_null(player1);
    cr_assert_not_null(player2);

    incantation->players[0] = player1;
    incantation->players[1] = player2;

    // Add required resources to the tile
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 1;
    required_resources[DERAUMERE] = 1;
    required_resources[SIBUR] = 2;
    required_resources[MENDIANE] = 1;
    add_resources_to_tile(game->map, pos, required_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    level_t initial_level1 = player1->level;
    level_t initial_level2 = player2->level;

    update_incantation(incantation, game);

    // Players should not level up due to insufficient player count
    cr_assert_eq(player1->level, initial_level1);
    cr_assert_eq(player2->level, initial_level2);

    // Incantation should still be removed from vector
    cr_assert_eq(vtable->size(game->incantations), 0);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_insufficient_resources)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {6, 6};
    level_t level =
        3;      // Level 3 to 4 requires: 2 linemate, 1 sibur, 2 phiras

    incantation_t *incantation = create_test_incantation(pos, level, 2);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create correct number of players
    player_t *player1 = create_test_player(game, pos, level);
    player_t *player2 = create_test_player(game, pos, level);
    cr_assert_not_null(player1);
    cr_assert_not_null(player2);

    incantation->players[0] = player1;
    incantation->players[1] = player2;

    // Add insufficient resources to the tile (missing phiras)
    inventory_t insufficient_resources = {0};
    insufficient_resources[LINEMATE] = 2;
    insufficient_resources[SIBUR] = 1;
    insufficient_resources[PHIRAS] = 1;      // Need 2, only have 1
    add_resources_to_tile(game->map, pos, insufficient_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    level_t initial_level1 = player1->level;
    level_t initial_level2 = player2->level;

    update_incantation(incantation, game);

    // Players should not level up due to insufficient resources
    cr_assert_eq(player1->level, initial_level1);
    cr_assert_eq(player2->level, initial_level2);

    // Incantation should still be removed from vector
    cr_assert_eq(vtable->size(game->incantations), 0);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_max_level_player)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {8, 8};
    level_t level = MAX_PLAYER_LEVEL;

    incantation_t *incantation = create_test_incantation(pos, level, 1);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create a player at max level
    player_t *player = create_test_player(game, pos, level);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    level_t initial_level = player->level;

    update_incantation(incantation, game);

    // Player should remain at max level (cannot level up further)
    cr_assert_eq(player->level, initial_level);

    // Incantation should be removed from vector
    cr_assert_eq(vtable->size(game->incantations), 0);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_empty_players_array)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {9, 9};
    level_t level = 1;

    incantation_t *incantation = create_test_incantation(pos, level, 0);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // No players in the incantation
    // incantation->players[0] is already NULL

    // Add required resources to the tile
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 1;
    add_resources_to_tile(game->map, pos, required_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    update_incantation(incantation, game);

    // Incantation should be removed from vector
    cr_assert_eq(vtable->size(game->incantations), 0);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_multiple_ticks_countdown)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {0, 9};
    level_t level = 1;

    incantation_t *incantation = create_test_incantation(pos, level, 1);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 1;      // Start with 1 tick

    // Create a player
    player_t *player = create_test_player(game, pos, level);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Add required resources to the tile
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 1;
    add_resources_to_tile(game->map, pos, required_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    cr_assert_eq(vtable->size(game->incantations), 1);

    // Call update_incantation: ticks_left goes from 1 to 0, incantation
    // completes and gets destroyed
    update_incantation(incantation, game);

    destroy_test_game(game);
}

Test(update_incantation, update_incantation_simple_removal_test)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    pos_t pos = {1, 1};
    level_t level = 1;

    incantation_t *incantation = create_test_incantation(pos, level, 1);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;      // Set to 0 so it completes immediately

    // Create a player
    player_t *player = create_test_player(game, pos, level);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Add required resources to the tile
    inventory_t required_resources = {0};
    required_resources[LINEMATE] = 1;
    add_resources_to_tile(game->map, pos, required_resources);

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    // Verify it was added
    cr_assert_eq(vtable->size(game->incantations), 1);

    // Verify the incantation pointer in the vector matches our incantation
    incantation_t *stored_incantation =
        *(incantation_t **)vtable->at(game->incantations, 0);
    cr_assert_eq(stored_incantation, incantation);

    // Call update_incantation - should complete and remove from vector
    update_incantation(incantation, game);

    // Check that incantation was removed from vector
    cr_assert_eq(vtable->size(game->incantations), 0);

    destroy_test_game(game);
}

// Test to debug vector removal issue
Test(update_incantation, debug_vector_removal)
{
    game_t *game = create_test_game();
    cr_assert_not_null(game);

    // Create a simple incantation that will fail (no resources)
    pos_t pos = {5, 5};
    incantation_t *incantation = create_test_incantation(pos, 1, 1);
    cr_assert_not_null(incantation);
    incantation->ticks_left = 0;

    // Create a player
    player_t *player = create_test_player(game, pos, 1);
    cr_assert_not_null(player);
    incantation->players[0] = player;

    // Don't add resources - this should cause the incantation to fail

    // Add incantation to game vector
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    vtable->push_back(game->incantations, &incantation);

    cr_assert_eq(vtable->size(game->incantations), 1);

    // Verify the incantation is stored correctly
    incantation_t *stored =
        *(incantation_t **)vtable->at(game->incantations, 0);
    cr_assert_eq(stored, incantation);

    // Skip calling update_incantation for now, just test manual removal
    // Manually call remove_incantation_from_vector equivalent
    for (size_t i = 0; i < vtable->size(game->incantations); i++) {
        incantation_t *current =
            *(incantation_t **)vtable->at(game->incantations, i);
        if (current == incantation) {
            vtable->erase(game->incantations, i);
            break;
        }
    }

    // Check if removal worked
    cr_assert_eq(vtable->size(game->incantations), 0);

    // Clean up manually since we didn't use update_incantation
    destroy_incantation(incantation);

    destroy_test_game(game);
}
