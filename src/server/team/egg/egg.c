/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg
*/

#include "team/egg/egg.h"
#include "debug_categories.h"
#include "map/coordinates.h"
#include "map/tile.h"
#include "team/team.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Create a new egg at a specified position with a team pointer.
 *
 * This function allocates memory for an egg structure, initializes its
 * position and team, and returns a pointer to the newly created egg.
 *
 * @param pos The position where the egg will be created
 * @param team Pointer to the team associated with the egg
 * @return Pointer to the newly created egg_t structure on success,
 *         NULL if memory allocation fails
 */
egg_t *create_egg(const pos_t pos, team_t *team, bool debug)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (egg == NULL) {
        perror("Failed to allocate memory for egg");
        return NULL;
    }
    egg->pos = pos;
    egg->team = team;
    add_egg_to_team(team, egg);
    debug_map(debug, "Egg created at (%zu, %zu) for team %s\n",
        pos.x, pos.y, team->name);
    return egg;
}

/**
 * @brief Free the memory allocated for an egg.
 *
 * This function deallocates the memory used by the egg structure.
 * It should be called when the egg is no longer needed to prevent memory
 * leaks.
 *
 * @param egg Pointer to the egg structure to be destroyed
 *
 * @note If the egg pointer is NULL, this function does nothing.
 */
void destroy_egg(egg_t *egg)
{
    if (egg == NULL)
        return;
    free(egg);
    egg = NULL;
}

/**
 * @brief Add an egg to a tile's egg list.
 *
 * This function adds an egg to the specified tile's eggs vector. If the tile
 * or egg pointer is NULL, it prints an error message and returns without
 * making any changes.
 *
 * @param tile Pointer to the tile structure where the egg will be added
 * @param egg Pointer to the egg structure to be added to the tile
 */
void add_egg_to_tile(tile_t *tile, egg_t *egg)
{
    if (tile == NULL || egg == NULL) {
        fprintf(stderr, "Invalid tile or egg pointer\n");
        return;
    }
    vector_get_vtable(tile->eggs)->push_back(tile->eggs, &egg);
}

/**
 * @brief Remove an egg from a tile's egg list.
 *
 * This function removes the specified egg from the tile's eggs vector. If the
 * tile or egg pointer is NULL, it prints an error message and returns without
 * making any changes. If the egg is not found on the tile, it also prints an
 * error message.
 *
 * @param tile Pointer to the tile structure from which the egg will be removed
 * @param egg Pointer to the egg structure to be removed from the tile
 */
void remove_egg_from_tile(tile_t *tile, egg_t *egg)
{
    const vector_vtable_t *vtable;
    egg_t *current_egg;

    if (tile == NULL || egg == NULL) {
        fprintf(stderr, "Invalid tile or egg pointer\n");
        return;
    }
    vtable = vector_get_vtable(tile->eggs);
    for (size_t i = 0; i < vtable->size(tile->eggs); i++) {
        current_egg = *(egg_t **)vtable->at(tile->eggs, i);
        if (current_egg == egg) {
            vtable->erase(tile->eggs, i);
            return;
        }
    }
    fprintf(stderr, "Egg not found on tile\n");
}
