/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look
*/

#include "map/map.h"
#include "map/tile.h"
#include "team/player/player.h"
#include "utils/string.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const pos_t directions[4] = {
    {0, -1},      // NORTH
    {1, 0},       // EAST
    {0, 1},       // SOUTH
    {-1, 0}       // WEST
};

static char *get_tile_contents(tile_t *tile)
{
    char *contents = empty_string();

    if (tile == NULL) {
        fprintf(stderr, "Invalid tile pointer\n");
        return NULL;
    }
    if (contents == NULL) {
        perror("Failed to allocate memory for empty string");
        return NULL;
    }
    return contents;
}

char *look(player_t *player, map_t *map)
{
    char *contents = empty_string();

    if (player == NULL || map == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return NULL;
    }
    if (contents == NULL) {
        perror("Failed to allocate memory for contents");
        return NULL;
    }
    dyn_strcat(contents, "[");
    if (player->level == 1)
        dyn_strcat(contents, get_tile_contents(get_tile(map, player->pos)));
    return contents;
}
