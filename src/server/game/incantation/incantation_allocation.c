/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Incantation Allocation
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "game/incantation.h"
#include "map/coordinates.h"
#include "map/tile.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Destroys an incantation and frees its resources
 *
 * This function deallocates memory for the players array within the
 * incantation and sets the incantation pointer to NULL to avoid dangling
 * pointers.
 *
 * @param incantation Pointer to the incantation to destroy (can be NULL)
 */
void destroy_incantation(incantation_t *incantation)
{
    if (incantation == NULL) {
        fprintf(stderr, "Invalid incantation pointer\n");
        return;
    }
    if (incantation->players != NULL) {
        free(incantation->players);
    }
    free(incantation);
    incantation = NULL;
}

/**
 * @brief Destroys a vector of incantations and frees all associated memory
 *
 * This function iterates through the vector of incantations, destroys each
 * incantation, and frees the memory allocated for each incantation pointer.
 * Finally, it destroys the vector itself.
 *
 * @param incantations Pointer to the vector of incantations to destroy
 */
void destroy_incantation_vector(vector_t *incantations)
{
    const vector_vtable_t *vtable = vector_get_vtable(incantations);
    size_t size = vtable->size(incantations);
    incantation_t *incantation = NULL;

    for (size_t i = 0; i < size; i++) {
        incantation = *(incantation_t **)vtable->at(incantations, i);
        if (incantation == NULL)
            continue;
        destroy_incantation(incantation);
    }
    vector_destroy(incantations);
}

/**
 * @brief Fills an array of players with those matching a specific level
 *
 * This function iterates through the players in a tile and fills the provided
 * array with pointers to players that match the specified level. The array is
 * terminated with a NULL pointer.
 *
 * @param players Array of player pointers to fill
 * @param tile Pointer to the tile containing players
 * @param level Level to match against players
 * @param vtable Pointer to the vector vtable for accessing players
 */
static void fill_players_array(player_t **players, tile_t *tile, level_t level,
    const vector_vtable_t *vtable)
{
    size_t count = 0;
    player_t *player = NULL;

    for (size_t i = 0; i < vtable->size(tile->players); i++) {
        player = *(player_t **)vtable->at(tile->players, i);
        player->in_incantation = true;
        player->tick_cooldown = INCANTATION_COMMAND_COOLDOWN;
        dprintf(player->client->sockfd, "Elevation underway\n");
        debug_player(player->client->server->options->debug,
            "Player %zu is in an incantation at (%d, %d) for level %u\n",
            player->id, player->pos.x, player->pos.y, level);
        if (player->level == level) {
            players[count] = player;
            count++;
        }
    }
    players[count] = NULL;
}

/**
 * @brief Initializes an array of players for an incantation at a given
 * position
 *
 * This function allocates memory for an array of player pointers, retrieves
 * the tile at the specified position, and fills the array with players that
 * match the specified level.
 *
 * @param game Pointer to the game structure containing map and teams
 * @param pos Position on the map where the incantation is taking place
 * @param level Level of the incantation
 * @return Pointer to an array of player pointers, or NULL on failure
 */
static player_t **init_players(game_t *game, pos_t pos, level_t level)
{
    const vector_vtable_t *vtable = vector_get_vtable(game->map->tiles);
    tile_t *tile = get_tile(game->map, pos);
    player_t **players =
        malloc(sizeof(player_t *) * (vtable->size(tile->players) + 1));

    if (players == NULL) {
        perror("Failed to allocate memory for players array");
        return NULL;
    }
    if (tile == NULL) {
        fprintf(stderr, "Tile not found at position (%d, %d)\n", pos.x, pos.y);
        free(players);
        return NULL;
    }
    fill_players_array(players, tile, level, vtable);
    return players;
}

/**
 * @brief Creates a new incantation at the specified position and level
 *
 * This function allocates memory for a new incantation, initializes its
 * players based on the game state, and adds it to the game's incantations
 * vector.
 *
 * @param game Pointer to the game structure containing incantations vector
 * @param pos Position of the incantation on the map
 * @param level Level of the incantation
 * @return Pointer to the created incantation, or NULL on failure
 */
incantation_t *create_incantation(game_t *game, pos_t pos, level_t level)
{
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    incantation_t *incantation = malloc(sizeof(incantation_t));

    if (incantation == NULL) {
        perror("Failed to allocate memory for incantation");
        return NULL;
    }
    incantation->players = init_players(game, pos, level);
    if (incantation->players == NULL) {
        fprintf(stderr, "Failed to initialize players for incantation\n");
        destroy_incantation(incantation);
        return NULL;
    }
    incantation->pos = pos;
    incantation->level = level;
    incantation->ticks_left = INCANTATION_COMMAND_COOLDOWN;
    pic_event(incantation, game->server);
    vtable->push_back(game->incantations, &incantation);
    return incantation;
}

/**
 * @brief Removes a player from an incantation by compacting the player array
 *
 * This function searches for the specified player in the incantation's player
 * array and removes them by shifting the remaining players down to fill the
 * gap, maintaining the NULL-terminated structure.
 *
 * @param incantation Pointer to the incantation structure containing players
 * @param player Pointer to the player to remove from the incantation
 * @param debug Boolean indicating whether to print debug information
 */
static void remove_player_from_incantation(
    incantation_t *incantation, player_t *player, bool debug)
{
    size_t write_idx = 0;
    bool found = false;

    for (size_t read_idx = 0; incantation->players[read_idx] != NULL;
        read_idx++) {
        if (incantation->players[read_idx] == player) {
            found = true;
            continue;
        }
        incantation->players[write_idx] = incantation->players[read_idx];
        write_idx++;
    }
    incantation->players[write_idx] = NULL;
    if (found) {
        debug_game(debug,
            "Removed player %zu from incantation at (%d, %d) for level "
            "%u -> %u\n",
            player->id, incantation->pos.x, incantation->pos.y,
            incantation->level, incantation->level + 1);
    }
}

/**
 * @brief Removes a player from all incantations by compacting the player
 * arrays
 *
 * This function iterates through all incantations in the game and removes any
 * occurrence of the specified player. The array is compacted to maintain the
 * NULL-terminated structure without holes.
 *
 * @param game Pointer to the game structure containing incantations vector
 * @param player Pointer to the player to remove from all incantations
 */
void remove_player_from_all_incantations(game_t *game, player_t *player)
{
    const vector_vtable_t *vtable;
    incantation_t *incantation = NULL;
    size_t incantations_count = 0;

    if (game == NULL || game->incantations == NULL || player == NULL) {
        return;
    }
    vtable = vector_get_vtable(game->incantations);
    incantations_count = vtable->size(game->incantations);
    for (size_t i = 0; i < incantations_count; i++) {
        incantation = *(incantation_t **)vtable->at(game->incantations, i);
        if (incantation == NULL || incantation->players == NULL) {
            continue;
        }
        remove_player_from_incantation(
            incantation, player, game->server->options->debug);
    }
}
