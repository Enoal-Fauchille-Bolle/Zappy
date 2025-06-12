/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#include "team/player/player.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Create a new player with a given position.
 *
 * This function allocates memory for a player structure, initializes its
 * position, orientation, level, and inventory. The player's orientation is set
 * to a random value between 1 and 4.
 *
 * @param pos The initial position of the player
 * @return Pointer to the newly created player_t structure on success,
 *         NULL if memory allocation fails
 */
player_t *create_player(const pos_t pos)
{
    player_t *player = malloc(sizeof(player_t));

    if (player == NULL) {
        perror("Failed to allocate memory for player");
        return NULL;
    }
    player->pos = pos;
    player->orientation = (rand() % 4) + 1;
    player->level = 1;
    for (size_t i = 0; i <= THYSTAME; i++)
        player->inventory[i] = 0;
    player->tick_cooldown = 0;
    return player;
}

/**
 * @brief Free the memory allocated for a player.
 *
 * This function deallocates the memory used by the player structure.
 * It should be called when the player is no longer needed to prevent memory
 * leaks.
 *
 * @param player Pointer to the player structure to be destroyed
 *
 * @note If the player pointer is NULL, this function does nothing.
 */
void destroy_player(player_t *player)
{
    if (player == NULL)
        return;
    free(player);
}

/**
 * @brief Add a player to a tile's player list.
 *
 * This function adds a player to the specified tile's player vector. If the
 * tile or player pointer is NULL, it prints an error message and returns
 * without making any changes.
 *
 * @param tile Pointer to the tile structure where the player will be added
 * @param player Pointer to the player structure to be added to the tile
 */
void add_player_to_tile(tile_t *tile, player_t *player)
{
    if (tile == NULL || player == NULL) {
        fprintf(stderr, "Invalid tile or player pointer\n");
        return;
    }
    vector_get_vtable(tile->players)->push_back(tile->players, &player);
}

/**
 * @brief Remove a player from a tile's player list.
 *
 * This function searches for a player in the specified tile's player vector
 * and removes them if found. If the tile or player pointer is NULL, it prints
 * an error message and returns without making any changes.
 *
 * @param tile Pointer to the tile structure from which the player will be
 * removed
 * @param player Pointer to the player structure to be removed from the tile
 */
void remove_player_from_tile(tile_t *tile, player_t *player)
{
    const vector_vtable_t *vtable;
    player_t *current_player;

    if (tile == NULL || player == NULL) {
        fprintf(stderr, "Invalid tile or player pointer\n");
        return;
    }
    vtable = vector_get_vtable(tile->players);
    for (size_t i = 0; i < vtable->size(tile->players); i++) {
        current_player = *(player_t **)vtable->at(tile->players, i);
        if (current_player == player) {
            vtable->erase(tile->players, i);
            return;
        }
    }
    fprintf(stderr, "Player not found on tile\n");
}
