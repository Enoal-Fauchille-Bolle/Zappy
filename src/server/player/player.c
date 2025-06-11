/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#include "player/player.h"
#include "map/coordinates.h"
#include "map/resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Create a new player with specified attributes.
 *
 * This function allocates memory for a player structure, initializes its
 * attributes, and sets a random orientation. The player starts at the given
 * position and belongs to the specified team.
 *
 * @param id Unique identifier for the player
 * @param pos Initial position of the player on the map
 * @param team_id Identifier for the team the player belongs to
 * @return Pointer to the initialized player_t structure on success,
 *         NULL if memory allocation fails
 *
 * @note Caller is responsible for freeing the returned structure using
 *       destroy_player() when it is no longer needed.
 * @note The player's orientation is randomly set to one of the four possible
 *       directions (NORTH, EAST, SOUTH, WEST) upon creation.
 * @note The player needs to be added to the map after creation using
 *       add_player_to_map() to be visible in the game world.
 */
player_t *create_player(size_t id, pos_t pos, size_t team_id)
{
    player_t *player = malloc(sizeof(player_t));

    if (player == NULL) {
        perror("Failed to allocate memory for player");
        return NULL;
    }
    player->id = id;
    player->pos = pos;
    player->orientation = (rand() % 4) + 1;
    player->level = 1;
    for (size_t i = 0; i <= THYSTAME; i++)
        player->inventory[i] = 0;
    player->team_id = team_id;
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
