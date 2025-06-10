/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#include "player/player.h"
#include "map/coordinates.h"
#include "map/map.h"
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
    srand(time(NULL));
    player->id = id;
    player->pos = pos;
    player->orientation = (rand() % 4) + 1;
    player->level = 1;
    player->inventory = (inventory_t){0};
    player->team_id = team_id;
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
 * @brief Turn the player to the left.
 *
 * This function updates the player's orientation to the left
 * (counter-clockwise) and sets a cooldown for the next action.
 *
 * @param player Pointer to the player structure whose orientation will be
 * changed
 */
void turn_player_left(player_t *player)
{
    player->orientation = turn_left(player->orientation);
    player->tick_cooldown = 7;
}

/**
 * @brief Turn the player to the right.
 *
 * This function updates the player's orientation to the right
 * (clockwise) and sets a cooldown for the next action.
 *
 * @param player Pointer to the player structure whose orientation will be
 * changed
 */
void turn_player_right(player_t *player)
{
    player->orientation = turn_right(player->orientation);
    player->tick_cooldown = 7;
}

/**
 * @brief Move the player forward in the direction they are facing.
 *
 * This function updates the player's position based on their current
 * orientation and the map's dimensions. If the player or map pointer is NULL,
 * it prints an error message and returns without making any changes.
 *
 * @param player Pointer to the player structure to be moved
 * @param map Pointer to the map structure where the player is located
 */
// TODO: Remove player pointer from the tile at the current position
//       Add player pointer to the tile at the new position
void move_player_forward(player_t *player, map_t *map)
{
    tile_t *current_tile = get_tile(map, player->pos);

    if (player == NULL || map == NULL || current_tile == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return;
    }
    remove_player_from_map(map, player);
    player->pos = get_forward_position(player->pos, player->orientation, map);
    player->tick_cooldown = 7;
    add_player_to_map(map, player);
}
