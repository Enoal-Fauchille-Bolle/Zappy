/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Ejection
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "debug_categories.h"
#include "map/coordinates.h"
#include "map/orientation_names.h"
#include "map/tile.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Destroys all eggs on a given tile.
 *
 * This function iterates through all eggs on the specified tile and destroys
 * each one, then clears the eggs vector.
 *
 * @param tile Pointer to the tile whose eggs are to be destroyed.
 */
static void destroy_eggs_on_tile(tile_t *tile, game_t *game)
{
    const vector_vtable_t *vtable = vector_get_vtable(tile->eggs);
    egg_t *egg = NULL;

    if (!tile || !tile->eggs) {
        fprintf(stderr, "Invalid tile or eggs vector\n");
        return;
    }
    for (size_t i = vtable->size(tile->eggs); i > 0; i--) {
        egg = *(egg_t **)vtable->at(tile->eggs, i - 1);
        if (!egg)
            continue;
        edi_event(egg);
        debug_map(game->server->options->debug,
            "Destroying egg %zu at (%ld, %ld) for team %s\n", egg->id,
            egg->pos.x, egg->pos.y, egg->team ? egg->team->name : "No Team");
        remove_egg_from_team(egg->team, egg);
        vtable->erase(tile->eggs, i - 1);
        destroy_egg(egg);
    }
}

/**
 * @brief Gets the target position for a player based on their orientation.
 *
 * This function calculates the target position for a player by moving them
 * forward in the direction they are facing, taking into account the map's
 * boundaries.
 *
 * @param player Pointer to the player whose target position is being
 * calculated.
 * @return The target position as a pos_t structure.
 */
static pos_t get_target_position(player_t *player)
{
    return get_forward_position(
        player->pos, player->orientation, player->client->server->game->map);
}

/**
 * @brief Reverses the orientation by adding 2 and wrapping around.
 *
 * This function takes an orientation and returns the opposite direction by
 * adding 2 to the current orientation value and wrapping it around if it goes
 * beyond WEST.
 *
 * @param orientation The current orientation.
 * @return The reversed orientation.
 */
static orientation_t reverse_orientation(orientation_t orientation)
{
    return orientation + 2 % ORIENTATION_COUNT;
}

/**
 * @brief Ejects a player from a tile to a target position.
 *
 * This function removes the current player from the tile, updates their
 * position to the target position, and adds them to the target tile. It also
 * sends a debug message and notifies the client about the ejection.
 *
 * @param tile Pointer to the tile from which the player is being ejected.
 * @param ejecting_player Pointer to the player who is ejecting others.
 * @param current_player Pointer to the player being ejected.
 * @param debug Boolean indicating whether to print debug information.
 */
static void eject_player_from_tile(tile_t *tile, player_t *ejecting_player,
    player_t *ejected_player, bool debug)
{
    pos_t pos = get_target_position(ejecting_player);
    tile_t *target_tile =
        get_tile(ejecting_player->client->server->game->map, pos);
    orientation_t orientation_origin =
        reverse_orientation(ejecting_player->orientation);

    if (!target_tile || !target_tile->players) {
        fprintf(stderr, "Invalid tile or players vector\n");
        return;
    }
    remove_player_from_tile(tile, ejected_player);
    ejected_player->pos = pos;
    add_player_to_tile(target_tile, ejected_player);
    debug_map(debug,
        "Player %zu got ejected from (%ld, %ld) to (%ld, %ld) towards "
        "%s\n",
        ejected_player->id, ejecting_player->pos.x, ejecting_player->pos.y,
        pos.x, pos.y, orientation_names[ejected_player->orientation]);
    dprintf(ejected_player->client->sockfd, "eject: %d\n", orientation_origin);
    ppo_event(ejected_player);
}

/**
 * @brief Ejects all players from a tile except the ejecting player.
 *
 * This function iterates through all players on the specified tile and ejects
 * them if they are not the ejecting player. It uses the
 * `eject_player_from_tile` function to handle the ejection logic.
 *
 * @param tile Pointer to the tile from which players will be ejected.
 * @param ejecting_player Pointer to the player who is ejecting others.
 * @param debug Boolean indicating whether to print debug information.
 */
static void eject_players_from_tile(
    tile_t *tile, player_t *ejecting_player, bool debug)
{
    const vector_vtable_t *vtable = vector_get_vtable(tile->players);
    player_t *current_player;

    for (size_t i = vtable->size(tile->players); i > 0; i--) {
        current_player = *(player_t **)vtable->at(tile->players, i - 1);
        if (!current_player)
            continue;
        if (current_player == ejecting_player)
            continue;
        eject_player_from_tile(tile, ejecting_player, current_player, debug);
    }
}

/**
 * @brief Ejects the player from their current tile.
 *
 * This function checks if the player can eject based on the presence of other
 * players or eggs on the same tile. If successful, it ejects all players from
 * the tile and destroys any eggs present.
 *
 * @param player Pointer to the player who is attempting to eject.
 * @param map Pointer to the game map where the player is located.
 * @return SUCCESS if ejection was successful, FAILURE otherwise.
 */
bool eject(player_t *player, map_t *map)
{
    tile_t *current_tile = NULL;

    if (!player || !map) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return FAILURE;
    }
    current_tile = get_tile(map, player->pos);
    if (!current_tile || (get_nb_players_on_tile(current_tile) <= 1 &&
        get_nb_eggs_on_tile(current_tile) <= 0)) {
        debug_warning(player->client->server->options->debug,
            "Player %zu cannot eject: no other players on tile\n", player->id);
        return FAILURE;
    }
    eject_players_from_tile(
        current_tile, player, player->client->server->options->debug);
    if (player->client->server->options->display_eggs &&
        get_nb_eggs_on_tile(current_tile) > 0) {
        destroy_eggs_on_tile(current_tile, player->client->server->game);
    }
    return SUCCESS;
}
