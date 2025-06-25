/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** tile_utils
*/

#include "map/tile.h"
#include "team/player/player.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>

/**
 * @brief Get the number of players on a tile with a specific level.
 *
 * This function counts the number of players on the specified tile that have
 * the given level. If the tile pointer is NULL, it prints an error message
 * and returns 0.
 *
 * @param tile Pointer to the tile_t structure from which to get the player
 * count
 * @param level The level of players to count
 * @return Number of players with the specified level on the tile, or 0 if the
 *         tile pointer is NULL.
 */
size_t get_nb_players_on_tile_with_level(const tile_t *tile, level_t level)
{
    const vector_vtable_t *vtable;
    size_t count = 0;
    player_t *current_player = NULL;

    if (tile == NULL) {
        fprintf(stderr, "Invalid tile pointer\n");
        return 0;
    }
    vtable = vector_get_vtable(tile->players);
    for (size_t i = 0; i < vtable->size(tile->players); i++) {
        current_player = *(player_t **)vtable->at(tile->players, i);
        if (current_player && current_player->level == level)
            count++;
    }
    return count;
}
