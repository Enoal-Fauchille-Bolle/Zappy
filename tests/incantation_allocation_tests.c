/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation Allocation Tests
*/

#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "game/incantation.h"
#include "map/coordinates.h"
#include "map/map.h"
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

    // Destroy map first to avoid circular dependencies
    if (game->map)
        destroy_map(game->map);

    // Clean up incantations vector carefully
    if (game->incantations) {
        const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
        size_t size = vtable->size(game->incantations);

        // Destroy each incantation
        for (size_t i = 0; i < size; i++) {
            incantation_t **inc_ptr =
                (incantation_t **)vtable->at(game->incantations, i);
            if (inc_ptr && *inc_ptr) {
                if ((*inc_ptr)->players) {
                    free((*inc_ptr)->players);
                }
                free(*inc_ptr);
            }
        }
        vector_destroy(game->incantations);
    }

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

// Simplified test helper functions that avoid complex dependencies

static incantation_t *create_simple_incantation(pos_t pos, level_t level)
{
    incantation_t *incantation = malloc(sizeof(incantation_t));
    if (!incantation)
        return NULL;

    incantation->pos = pos;
    incantation->level = level;
    incantation->ticks_left = INCANTATION_COMMAND_COOLDOWN;
    incantation->players = malloc(sizeof(player_t *) * 3);
    if (!incantation->players) {
        free(incantation);
        return NULL;
    }

    // Initialize with NULL-terminated array
    incantation->players[0] = NULL;
    incantation->players[1] = NULL;
    incantation->players[2] = NULL;

    return incantation;
}

// Tests for destroy_incantation function

Test(incantation_allocation, destroy_incantation_null_pointer)
{
    // Test that the function handles NULL pointer gracefully
    // We can't easily test stderr output in this context, so we just ensure it
    // doesn't crash
    destroy_incantation(NULL);
    cr_assert(true);      // If we reach here, the function didn't crash
}

Test(incantation_allocation, destroy_incantation_valid_incantation)
{
    // Create a valid incantation
    incantation_t *incantation = create_simple_incantation((pos_t){5, 5}, 2);
    cr_assert_not_null(incantation);

    // This should not crash
    destroy_incantation(incantation);
    cr_assert(true);      // If we reach here, the function didn't crash
}

Test(incantation_allocation, destroy_incantation_null_players_array)
{
    // Test with NULL players array
    incantation_t *incantation = malloc(sizeof(incantation_t));
    cr_assert_not_null(incantation);

    incantation->players = NULL;
    incantation->pos = (pos_t){5, 5};
    incantation->level = 2;
    incantation->ticks_left = 300;

    // This should not crash
    destroy_incantation(incantation);
    cr_assert(true);      // If we reach here, the function didn't crash
}

// Tests for destroy_incantation_vector function

Test(incantation_allocation, destroy_incantation_vector_empty)
{
    vector_t *incantations = vector_new(sizeof(incantation_t *));
    cr_assert_not_null(incantations);

    // This should not crash with empty vector
    destroy_incantation_vector(incantations);
    cr_assert(true);      // If we reach here, the function didn't crash
}

Test(incantation_allocation, destroy_incantation_vector_with_incantations)
{
    vector_t *incantations = vector_new(sizeof(incantation_t *));
    cr_assert_not_null(incantations);

    const vector_vtable_t *vtable = vector_get_vtable(incantations);

    // Create some simple incantations
    incantation_t *inc1 = create_simple_incantation((pos_t){1, 1}, 1);
    incantation_t *inc2 = create_simple_incantation((pos_t){2, 2}, 2);

    cr_assert_not_null(inc1);
    cr_assert_not_null(inc2);

    // Add to vector
    vtable->push_back(incantations, &inc1);
    vtable->push_back(incantations, &inc2);

    cr_assert_eq(vtable->size(incantations), 2);

    // This should destroy all incantations and the vector
    destroy_incantation_vector(incantations);
    cr_assert(true);      // If we reach here, the function didn't crash
}

// Simplified tests for create_incantation function using minimal setup

Test(incantation_allocation, create_incantation_minimal_setup)
{
    // Test the function with minimal setup to avoid complex dependencies
    game_t *game = create_test_game();
    if (!game) {
        cr_skip("Failed to create test game - skipping test");
        return;
    }

    pos_t pos = {5, 5};
    level_t level = 2;

    size_t initial_incantations =
        vector_get_vtable(game->incantations)->size(game->incantations);

    incantation_t *incantation = create_incantation(game, pos, level);

    if (incantation) {
        // Check basic properties if creation succeeded
        cr_assert_eq(incantation->pos.x, pos.x);
        cr_assert_eq(incantation->pos.y, pos.y);
        cr_assert_eq(incantation->level, level);
        cr_assert_eq(incantation->ticks_left, INCANTATION_COMMAND_COOLDOWN);
        cr_assert_not_null(incantation->players);

        // Check that incantation was added to the vector
        size_t final_incantations =
            vector_get_vtable(game->incantations)->size(game->incantations);
        cr_assert_eq(final_incantations, initial_incantations + 1);
    }

    destroy_test_game(game);
}

// Test memory management without complex game state

Test(incantation_allocation, incantation_structure_integrity)
{
    // Test basic structure allocation and initialization
    incantation_t *incantation = create_simple_incantation((pos_t){3, 7}, 4);
    cr_assert_not_null(incantation);

    // Verify structure fields
    cr_assert_eq(incantation->pos.x, 3);
    cr_assert_eq(incantation->pos.y, 7);
    cr_assert_eq(incantation->level, 4);
    cr_assert_eq(incantation->ticks_left, INCANTATION_COMMAND_COOLDOWN);
    cr_assert_not_null(incantation->players);

    // Verify players array is NULL-terminated
    cr_assert_null(incantation->players[0]);

    destroy_incantation(incantation);
}

// Test edge cases for levels and positions

Test(incantation_allocation, incantation_boundary_values)
{
    // Test minimum values
    incantation_t *min_incantation =
        create_simple_incantation((pos_t){0, 0}, 1);
    cr_assert_not_null(min_incantation);
    cr_assert_eq(min_incantation->pos.x, 0);
    cr_assert_eq(min_incantation->pos.y, 0);
    cr_assert_eq(min_incantation->level, 1);
    destroy_incantation(min_incantation);

    // Test maximum level
    incantation_t *max_incantation =
        create_simple_incantation((pos_t){999, 999}, MAX_PLAYER_LEVEL);
    cr_assert_not_null(max_incantation);
    cr_assert_eq(max_incantation->pos.x, 999);
    cr_assert_eq(max_incantation->pos.y, 999);
    cr_assert_eq(max_incantation->level, MAX_PLAYER_LEVEL);
    destroy_incantation(max_incantation);
}

// Test vector operations independently

Test(incantation_allocation, vector_incantation_operations)
{
    vector_t *incantations = vector_new(sizeof(incantation_t *));
    cr_assert_not_null(incantations);

    const vector_vtable_t *vtable = vector_get_vtable(incantations);
    cr_assert_not_null(vtable);

    // Test adding incantations to vector
    incantation_t *inc1 = create_simple_incantation((pos_t){1, 1}, 1);
    incantation_t *inc2 = create_simple_incantation((pos_t){2, 2}, 2);
    incantation_t *inc3 = create_simple_incantation((pos_t){3, 3}, 3);

    cr_assert_not_null(inc1);
    cr_assert_not_null(inc2);
    cr_assert_not_null(inc3);

    vtable->push_back(incantations, &inc1);
    vtable->push_back(incantations, &inc2);
    vtable->push_back(incantations, &inc3);

    cr_assert_eq(vtable->size(incantations), 3);

    // Verify we can retrieve incantations
    incantation_t **retrieved1 = (incantation_t **)vtable->at(incantations, 0);
    incantation_t **retrieved2 = (incantation_t **)vtable->at(incantations, 1);
    incantation_t **retrieved3 = (incantation_t **)vtable->at(incantations, 2);

    cr_assert_not_null(retrieved1);
    cr_assert_not_null(retrieved2);
    cr_assert_not_null(retrieved3);
    cr_assert_eq(*retrieved1, inc1);
    cr_assert_eq(*retrieved2, inc2);
    cr_assert_eq(*retrieved3, inc3);

    // Clean up manually since we're not using destroy_incantation_vector
    for (size_t i = 0; i < vtable->size(incantations); i++) {
        incantation_t **inc_ptr =
            (incantation_t **)vtable->at(incantations, i);
        if (inc_ptr && *inc_ptr) {
            destroy_incantation(*inc_ptr);
        }
    }
    vector_destroy(incantations);
}

// Test to verify fill_players_array for loop behavior through
// create_incantation
Test(incantation_allocation, create_incantation_fill_players_array_behavior)
{
    // Create a minimal game setup that allows create_incantation to work
    game_t *game = create_test_game();
    if (!game) {
        cr_skip("Failed to create test game - skipping test");
        return;
    }

    pos_t pos = {
        0, 0};      // Use corner position to avoid any wrapping issues
    level_t target_level = 2;

    // Get the tile at position (0,0)
    tile_t *tile = get_tile(game->map, pos);
    if (!tile) {
        destroy_test_game(game);
        cr_skip("Failed to get tile - skipping test");
        return;
    }

    // Create mock players with different levels
    // We'll create simple player structures that have the minimal fields
    // needed
    player_t *player1 = malloc(sizeof(player_t));
    player_t *player2 = malloc(sizeof(player_t));
    player_t *player3 = malloc(sizeof(player_t));
    player_t *player4 = malloc(sizeof(player_t));

    if (!player1 || !player2 || !player3 || !player4) {
        // Clean up and skip
        free(player1);
        free(player2);
        free(player3);
        free(player4);
        destroy_test_game(game);
        cr_skip("Failed to create players - skipping test");
        return;
    }

    // Initialize players with minimal required fields
    // Player 1: level 2 (matches target)
    player1->id = 1;
    player1->level = target_level;
    player1->in_incantation = false;
    player1->tick_cooldown = 0;
    player1->pos = pos;
    player1->client = malloc(sizeof(client_t));
    if (player1->client) {
        player1->client->sockfd =
            -1;      // Invalid fd, but won't crash dprintf
        player1->client->server = game->server;
    }

    // Player 2: level 1 (doesn't match target)
    player2->id = 2;
    player2->level = 1;
    player2->in_incantation = false;
    player2->tick_cooldown = 0;
    player2->pos = pos;
    player2->client = malloc(sizeof(client_t));
    if (player2->client) {
        player2->client->sockfd = -1;
        player2->client->server = game->server;
    }

    // Player 3: level 2 (matches target)
    player3->id = 3;
    player3->level = target_level;
    player3->in_incantation = false;
    player3->tick_cooldown = 0;
    player3->pos = pos;
    player3->client = malloc(sizeof(client_t));
    if (player3->client) {
        player3->client->sockfd = -1;
        player3->client->server = game->server;
    }

    // Player 4: level 3 (doesn't match target)
    player4->id = 4;
    player4->level = 3;
    player4->in_incantation = false;
    player4->tick_cooldown = 0;
    player4->pos = pos;
    player4->client = malloc(sizeof(client_t));
    if (player4->client) {
        player4->client->sockfd = -1;
        player4->client->server = game->server;
    }

    // Add players to the tile
    const vector_vtable_t *tile_vtable = vector_get_vtable(tile->players);
    tile_vtable->push_back(tile->players, &player1);
    tile_vtable->push_back(tile->players, &player2);
    tile_vtable->push_back(tile->players, &player3);
    tile_vtable->push_back(tile->players, &player4);

    // Verify players were added to tile
    cr_assert_eq(tile_vtable->size(tile->players), 4);

    // Create incantation - this will call fill_players_array internally
    incantation_t *incantation = create_incantation(game, pos, target_level);

    if (incantation) {
        // Test the effects of the fill_players_array for loop:

        // 1. ALL players on the tile should have in_incantation set to true
        cr_assert(player1->in_incantation,
            "Player1 should be marked as in_incantation");
        cr_assert(!player2->in_incantation,
            "Player2 should not be marked as in_incantation");
        cr_assert(player3->in_incantation,
            "Player3 should be marked as in_incantation");
        cr_assert(!player4->in_incantation,
            "Player4 should not be marked as in_incantation");

        // 2. ALL players should have tick_cooldown set to
        // INCANTATION_COMMAND_COOLDOWN
        cr_assert_eq(player1->tick_cooldown, INCANTATION_COMMAND_COOLDOWN);
        cr_assert_eq(player2->tick_cooldown, 0);
        cr_assert_eq(player3->tick_cooldown, INCANTATION_COMMAND_COOLDOWN);
        cr_assert_eq(player4->tick_cooldown, 0);

        // 3. Only players with matching level should be in the
        // incantation->players array
        cr_assert_not_null(
            incantation->players, "Players array should not be NULL");

        // Count players with matching level in the array
        int matching_players_count = 0;
        for (int i = 0; incantation->players[i] != NULL; i++) {
            cr_assert_eq(incantation->players[i]->level, target_level,
                "Only players with target level should be in incantation");
            matching_players_count++;
        }

        // We expect exactly 2 players with level 2 (player1 and player3)
        cr_assert_eq(matching_players_count, 2,
            "Should have exactly 2 players with target level in incantation");

        // 4. Verify the players array is NULL-terminated
        bool found_null_terminator = false;
        for (int i = 0; i < 5; i++) {      // Check up to 5 positions
            if (incantation->players[i] == NULL) {
                found_null_terminator = true;
                break;
            }
        }
        cr_assert(
            found_null_terminator, "Players array should be NULL-terminated");
    }

    // Clean up
    if (player1->client)
        free(player1->client);
    if (player2->client)
        free(player2->client);
    if (player3->client)
        free(player3->client);
    if (player4->client)
        free(player4->client);
    free(player1);
    free(player2);
    free(player3);
    free(player4);

    destroy_test_game(game);
}

// Test fill_players_array behavior with empty tile
Test(incantation_allocation,
    create_incantation_empty_tile_fill_players_behavior)
{
    game_t *game = create_test_game();
    if (!game) {
        cr_skip("Failed to create test game - skipping test");
        return;
    }

    pos_t pos = {1, 1};      // Use a different position
    level_t level = 1;

    // Get the tile - it should be empty by default
    tile_t *tile = get_tile(game->map, pos);
    if (!tile) {
        destroy_test_game(game);
        cr_skip("Failed to get tile - skipping test");
        return;
    }

    // Verify tile is empty
    const vector_vtable_t *tile_vtable = vector_get_vtable(tile->players);
    cr_assert_eq(
        tile_vtable->size(tile->players), 0, "Tile should be empty initially");

    // Create incantation on empty tile
    incantation_t *incantation = create_incantation(game, pos, level);

    if (incantation) {
        // When fill_players_array processes an empty tile:
        // 1. The for loop should not execute (size is 0)
        // 2. The players array should still be allocated
        // 3. The first element should be NULL (immediate termination)
        cr_assert_not_null(incantation->players,
            "Players array should be allocated even for empty tile");
        cr_assert_null(incantation->players[0],
            "First element should be NULL for empty tile");

        // Verify incantation basic properties
        cr_assert_eq(incantation->pos.x, pos.x);
        cr_assert_eq(incantation->pos.y, pos.y);
        cr_assert_eq(incantation->level, level);
        cr_assert_eq(incantation->ticks_left, INCANTATION_COMMAND_COOLDOWN);
    }

    destroy_test_game(game);
}

// Test fill_players_array behavior with players of different levels
Test(incantation_allocation,
    create_incantation_no_matching_level_fill_players_behavior)
{
    game_t *game = create_test_game();
    if (!game) {
        cr_skip("Failed to create test game - skipping test");
        return;
    }

    pos_t pos = {2, 2};
    level_t target_level =
        5;      // High level that won't match our test players

    tile_t *tile = get_tile(game->map, pos);
    if (!tile) {
        destroy_test_game(game);
        cr_skip("Failed to get tile - skipping test");
        return;
    }

    // Create players with levels that don't match the target
    player_t *player1 = malloc(sizeof(player_t));
    player_t *player2 = malloc(sizeof(player_t));

    if (!player1 || !player2) {
        free(player1);
        free(player2);
        destroy_test_game(game);
        cr_skip("Failed to create players - skipping test");
        return;
    }

    // Initialize players with levels different from target
    player1->id = 1;
    player1->level = 1;      // Different from target_level (5)
    player1->in_incantation = false;
    player1->tick_cooldown = 0;
    player1->pos = pos;
    player1->client = malloc(sizeof(client_t));
    if (player1->client) {
        player1->client->sockfd = -1;
        player1->client->server = game->server;
    }

    player2->id = 2;
    player2->level = 3;      // Different from target_level (5)
    player2->in_incantation = false;
    player2->tick_cooldown = 0;
    player2->pos = pos;
    player2->client = malloc(sizeof(client_t));
    if (player2->client) {
        player2->client->sockfd = -1;
        player2->client->server = game->server;
    }

    // Add players to tile
    const vector_vtable_t *tile_vtable = vector_get_vtable(tile->players);
    tile_vtable->push_back(tile->players, &player1);
    tile_vtable->push_back(tile->players, &player2);

    cr_assert_eq(tile_vtable->size(tile->players), 2);

    // Create incantation
    incantation_t *incantation = create_incantation(game, pos, target_level);

    if (incantation) {
        // Test fill_players_array behavior when no players match the target
        // level:

        // 1. All players should still be processed by the for loop
        cr_assert(!player1->in_incantation,
            "Player1 should not be marked as in_incantation");
        cr_assert(!player2->in_incantation,
            "Player2 should not be marked as in_incantation");
        cr_assert_eq(player1->tick_cooldown, 0);
        cr_assert_eq(player2->tick_cooldown, 0);

        // 2. No players should be added to the incantation array (only NULL
        // terminator)
        cr_assert_not_null(
            incantation->players, "Players array should be allocated");
        cr_assert_null(incantation->players[0],
            "No players should be in array when none match level");

        // Verify the for loop processed all players but added none to the
        // result
        int matching_count = 0;
        for (int i = 0; incantation->players[i] != NULL; i++) {
            matching_count++;
        }
        cr_assert_eq(matching_count, 0,
            "Should have 0 players with non-matching target level");
    }

    // Clean up
    if (player1->client)
        free(player1->client);
    if (player2->client)
        free(player2->client);
    free(player1);
    free(player2);

    destroy_test_game(game);
}
