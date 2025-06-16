/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/

#include "team/player/player.h"
#include "map/coordinates.h"
#include "map/resources.h"
#include "map/tile.h"
#include "team/egg/egg.h"
#include "team/team.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Create a new player at a specified position with a team pointer.
 *
 * This function allocates memory for a player structure, initializes its
 * position, ID, orientation, level, inventory, and adds the player to the
 * specified team.
 *
 * @param pos The position where the player will be created
 * @param id The unique identifier for the player
 * @param team Pointer to the team associated with the player
 * @return Pointer to the newly created player_t structure on success,
 *         NULL if memory allocation fails
 */
player_t *create_player(const pos_t pos, const size_t id, team_t *team)
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
    player->inventory[FOOD] = 10;
    player->hunger_cooldown = 0;
    player->tick_cooldown = 0;
    add_player_to_team(team, player);
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
    if (player->team != NULL) {
        remove_player_from_team(player->team, player);
    }
    free(player);
    player = NULL;
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

/**
 * @brief Spawn an egg at the player's position and add it to the map.
 *
 * This function creates a new egg at the player's current position and
 * associates it with the player's team. The egg is then added to the map's
 * egg list.
 *
 * This is the fork command for the player to lay an egg. It gives the player
 * a cooldown of 42 ticks before they can do another action.
 *
 * @param player Pointer to the player who is laying the egg
 * @param map Pointer to the map where the egg will be added
 * @return Pointer to the newly created egg_t structure on success,
 *         NULL if player or map is NULL, or if egg creation fails
 */
egg_t *lay_egg(player_t *player, map_t *map)
{
    egg_t *egg;

    if (player == NULL || map == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return NULL;
    }
    if (player->team == NULL) {
        fprintf(stderr, "Player is not part of a team\n");
        return NULL;
    }
    egg = create_egg(player->pos, player->team);
    if (egg == NULL) {
        fprintf(stderr, "Failed to create egg\n");
        return NULL;
    }
    add_egg_to_map(map, egg);
    player->tick_cooldown = 42;
    return egg;
}
