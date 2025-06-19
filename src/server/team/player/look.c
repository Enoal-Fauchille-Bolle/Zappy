/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look
*/

#include "map/coordinates.h"
#include "map/map.h"
#include "map/resource_names.h"
#include "map/tile.h"
#include "team/player/player.h"
#include "utils/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *dyn_strcat_free(char **original_dest, char *src)
{
    char *result = dyn_strcat(original_dest, src);

    free(src);
    return result;
}

/**
 * @brief Concatenate resources from a tile into a string.
 *
 * This function iterates through all resource types and appends their names
 * and quantities to the provided contents string. If the last character of
 * the contents is not a space and the resource quantity is greater than zero,
 * it adds a space before appending the resource name and quantity.
 *
 * @param contents Pointer to the string where resources will be concatenated.
 * @param tile Pointer to the tile containing resources.
 */
static void concat_resources(char **contents, tile_t *tile)
{
    if (*contents == NULL || tile == NULL) {
        fprintf(stderr, "Invalid contents or tile pointer\n");
        return;
    }
    for (int i = 0; i <= THYSTAME; i++) {
        if (strlen(*contents) > 0 &&
            (*contents)[strlen(*contents) - 1] != ' ' &&
            tile->resources[i] > 0)
            dyn_strcat(contents, " ");
        dyn_strcat_free(contents,
            repeat_string(resource_names[i], " ", tile->resources[i]));
    }
}

/**
 * @brief Get the contents of a tile as a string.
 *
 * This function retrieves the number of players on the tile and concatenates
 * it with the resources present on that tile. It returns a dynamically
 * allocated string containing this information.
 *
 * @param tile Pointer to the tile whose contents are being retrieved.
 * @return A dynamically allocated string containing the tile's contents, or
 * NULL if an error occurs.
 */
static char *get_tile_contents(tile_t *tile)
{
    char *contents = empty_string(0);

    if (tile == NULL) {
        fprintf(stderr, "Invalid tile pointer\n");
        free(contents);
        return NULL;
    }
    if (!contents)
        return NULL;
    dyn_strcat_free(
        &contents, repeat_string("player", " ", get_nb_players_on_tile(tile)));
    concat_resources(&contents, tile);
    return contents;
}

/**
 * @brief Add the contents of a tile to a list of contents.
 *
 * This function retrieves the contents of a tile and appends it to the
 * provided contents string, ensuring that the contents are separated by a
 * comma.
 *
 * @param tile Pointer to the tile whose contents are being added.
 * @param contents Pointer to the string where the tile's contents will be
 * appended.
 */
static void add_tile_to_list(tile_t *tile, char **contents)
{
    char *tile_content = get_tile_contents(tile);

    if (tile_content == NULL) {
        fprintf(stderr, "Failed to get tile contents\n");
        return;
    }
    dyn_strcat(contents, ",");
    dyn_strcat_free(contents, tile_content);
}

/**
 * @brief Get the tile at a specific offset from the player's position.
 *
 * This function calculates the position of a tile based on the player's
 * current position and orientation, and returns the tile at that position.
 * The depth and width parameters determine where from the player's position.
 *
 * @param map Pointer to the map structure containing the tiles.
 * @param player Pointer to the player structure whose position is being used.
 * @param depth The depth offset from the player's position.
 * @param width The width offset from the player's position.
 * @return Pointer to the tile at the calculated position, or NULL if not
 * found.
 */
static tile_t *get_tile_at_offset(
    map_t *map, player_t *player, size_t depth, size_t width)
{
    pos_t pos = get_relative_position(player->pos, player->orientation,
        (pos_t){(long)(width - depth), (long)depth}, map);

    return get_tile(map, pos);
}

/**
 * @brief Look around the player's current tile and return their contents.
 *
 * This function retrieves the contents of the tiles where the player is
 * located and ahead depending on level, including the resources present and
 * the number of players on those tiles. It returns a string representation of
 * these contents enclosed in square brackets. If the player is at level 1, it
 * includes only the first tile's contents.
 *
 * @param player Pointer to the player structure whose position is being looked
 * at.
 * @param map Pointer to the map structure containing the tiles.
 * @return A dynamically allocated string containing the tile's contents,
 *         or NULL if an error occurs.
 */
char *look(player_t *player, map_t *map)
{
    char *contents = empty_string(1);

    if (player == NULL || map == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        free(contents);
        return NULL;
    }
    if (!contents)
        return NULL;
    strcat(contents, "[");
    dyn_strcat_free(&contents, get_tile_contents(get_tile(map, player->pos)));
    for (size_t depth = 1; depth < player->level + 1; depth++) {
        for (size_t width = 0; width < depth * 2 + 1; width++)
            add_tile_to_list(
                get_tile_at_offset(map, player, depth, width), &contents);
    }
    dyn_strcat(&contents, "]");
    return contents;
}
