/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** egg
*/

#include "team/egg/egg.h"
#include "command_handler/gui_commands.h"
#include "debug_categories.h"
#include "game/game.h"
#include "map/coordinates.h"
#include "map/tile.h"
#include "team/team.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Initializes an egg structure with the provided parameters
 *
 * Sets up an egg by assigning it a unique ID, position, parent player ID,
 * and associating it with a team. The egg ID is automatically incremented
 * from the game's next available egg ID.
 *
 * @param egg Pointer to the egg structure to initialize
 * @param pos Position where the egg will be placed
 * @param team Pointer to the team that owns this egg
 * @param parent_id ID of the player that created this egg
 */
static void setup_egg(
    egg_t *egg, const pos_t pos, team_t *team, player_id_t parent_id)
{
    if (team != NULL) {
        egg->id = team->game->next_egg_id;
        team->game->next_egg_id++;
    } else {
        egg->id = 0;
    }
    egg->parent_id = parent_id;
    egg->pos = pos;
    egg->team = team;
}

/**
 * @brief Create a new egg at a specified position with a team pointer.
 *
 * This function allocates memory for an egg structure, initializes its
 * position and team, and returns a pointer to the newly created egg.
 *
 * @param pos The position where the egg will be created
 * @param team Pointer to the team associated with the egg
 * @param debug Boolean flag to enable debug messages
 * @return Pointer to the newly created egg_t structure on success,
 *         NULL if memory allocation fails
 */
egg_t *create_egg(
    const pos_t pos, team_t *team, player_id_t parent_id, bool debug)
{
    egg_t *egg = malloc(sizeof(egg_t));

    if (egg == NULL) {
        perror("Failed to allocate memory for egg");
        return NULL;
    }
    setup_egg(egg, pos, team, parent_id);
    if (team == NULL) {
        perror("Team pointer is NULL, egg will not be associated with a team");
    } else {
        add_egg_to_team(team, egg);
        if (team->name)
            debug_map(debug, "Egg %zu created at (%zu, %zu) for team %s\n",
                egg->id, pos.x, pos.y, team->name);
    }
    enw_event(egg);
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
