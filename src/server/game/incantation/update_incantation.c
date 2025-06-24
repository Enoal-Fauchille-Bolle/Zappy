/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Update Incantation
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug_categories.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "game/incantation.h"
#include "map/resources.h"
#include "map/tile.h"
#include "vector.h"
#include <stdio.h>

/**
 * @brief Remove an incantation from the vector of incantations
 *
 * This function searches for the specified incantation in the vector and
 * removes it if found. It uses the vector's vtable to access its methods.
 *
 * @param incantations Pointer to the vector containing incantations
 * @param incantation Pointer to the incantation to remove
 */
static void remove_incantation_from_vector(
    vector_t *incantations, incantation_t *incantation)
{
    const vector_vtable_t *vtable = vector_get_vtable(incantations);
    incantation_t *current = NULL;

    for (size_t i = 0; i < vtable->size(incantations); i++) {
        current = *(incantation_t **)vtable->at(incantations, i);
        if (current == incantation) {
            vtable->erase(incantations, i);
            return;
        }
    }
}

/**
 * @brief Level up players involved in the incantation
 *
 * This function iterates through the players participating in the incantation,
 * increments their level, and sends a message to each player indicating their
 * new level.
 *
 * @param incantation Pointer to the incantation structure containing players
 */
static void level_up_players(incantation_t *incantation)
{
    player_t *player = NULL;

    for (size_t i = 0; incantation->players[i] != NULL; i++) {
        player = incantation->players[i];
        player->level = incantation->level + 1;
        dprintf(player->client->sockfd, "Current level: %u\n", player->level);
        plv_event(player);
        debug_player(player->client->server->options->debug,
            "Player %zu has leveled up to level %u\n", player->id,
            player->level);
    }
}

/**
 * @brief Send a failure message to all players involved in the incantation
 *
 * This function sends a "ko" message to each player participating in the
 * incantation, indicating that the elevation has failed.
 *
 * @param incantation Pointer to the incantation structure containing players
 */
static void send_elevation_failed_to_players(incantation_t *incantation)
{
    for (size_t i = 0; incantation->players[i] != NULL; i++) {
        dprintf(incantation->players[i]->client->sockfd, "ko\n");
    }
}

/**
 * @brief Remove resources from the tile where the incantation took place
 *
 * This function retrieves the tile at the incantation's position and removes
 * the required resources based on the incantation's level. It prints an error
 * message if the tile is not found.
 *
 * @param incantation Pointer to the incantation structure containing position
 * @param map Pointer to the game map
 */
static void remove_stones(incantation_t *incantation, map_t *map)
{
    tile_t *tile = get_tile(map, incantation->pos);

    if (tile == NULL) {
        fprintf(stderr, "Tile not found at position (%d, %d)\n",
            incantation->pos.x, incantation->pos.y);
        return;
    }
    for (resource_t resource = FOOD; resource < RESOURCE_COUNT; resource++) {
        tile->resources[resource] -=
            INCANT_REQS[incantation->level - 1][resource];
    }
}

/**
 * @brief Complete the incantation by checking requirements and leveling up
 * players
 *
 * This function checks if the incantation requirements are met, removes the
 * required resources, levels up the players involved, and sends a success or
 * failure message to the players.
 *
 * @param incantation Pointer to the incantation structure
 * @param game Pointer to the game structure containing map and server
 */
static void complete_incantation(incantation_t *incantation, game_t *game)
{
    if (check_incantation_requirements(
            game->map, incantation->pos, incantation->level)) {
        remove_stones(incantation, game->map);
        level_up_players(incantation);
        debug_game(game->server->options->debug,
            "Incantation at (%d, %d) completed for level %u\n",
            incantation->pos.x, incantation->pos.y, incantation->level);
    } else {
        send_elevation_failed_to_players(incantation);
        debug_game(game->server->options->debug,
            "Incantation at (%d, %d) failed for level %u\n",
            incantation->pos.x, incantation->pos.y, incantation->level);
    }
}

/**
 * @brief Update the incantation by decrementing ticks and completing it if
 * necessary
 *
 * This function decrements the ticks left for the incantation. If the ticks
 * reach zero, it completes the incantation, removes it from the vector, and
 * destroys it.
 *
 * @param incantation Pointer to the incantation structure to update
 * @param game Pointer to the game structure containing incantations vector
 */
void update_incantation(incantation_t *incantation, game_t *game)
{
    if (incantation == NULL) {
        fprintf(stderr, "Invalid incantation pointer\n");
        return;
    }
    if (incantation->ticks_left > 0) {
        incantation->ticks_left--;
        return;
    }
    complete_incantation(incantation, game);
    remove_incantation_from_vector(game->incantations, incantation);
    destroy_incantation(incantation);
}
