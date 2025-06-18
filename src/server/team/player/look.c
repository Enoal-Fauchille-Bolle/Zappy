/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** look
*/

#include "map/map.h"
#include "map/resource_names.h"
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

static void concat_resources(char **contents, tile_t *tile)
{
    char *temp;

    for (int i = 0; i <= THYSTAME; i++) {
        temp = repeat_string(resource_names[i], " ", tile->resources[i]);
        if ((*contents)[strlen(*contents) - 1] != ' ' &&
            tile->resources[i] > 0)
            *contents = dyn_strcat(*contents, " ");
        *contents = dyn_strcat(*contents, temp);
        free(temp);
    }
}

static char *get_tile_contents(tile_t *tile)
{
    char *contents = empty_string();
    char *temp;

    if (tile == NULL) {
        fprintf(stderr, "Invalid tile pointer\n");
        return NULL;
    }
    if (contents == NULL) {
        perror("Failed to allocate memory for empty string");
        return NULL;
    }
    temp = repeat_string("player", " ", get_nb_players_on_tile(tile));
    contents = dyn_strcat(contents, temp);
    free(temp);
    concat_resources(&contents, tile);
    return contents;
}

char *look(player_t *player, map_t *map)
{
    char *contents = empty_string();
    char *tile_content;

    if (player == NULL || map == NULL) {
        fprintf(stderr, "Invalid player or map pointer\n");
        return NULL;
    }
    if (contents == NULL) {
        perror("Failed to allocate memory for contents");
        return NULL;
    }
    contents = dyn_strcat(contents, "[");
    if (player->level == 1) {
        tile_content = get_tile_contents(get_tile(map, player->pos));
        contents = dyn_strcat(contents, tile_content);
        free(tile_content);
    }
    contents = dyn_strcat(contents, "]");
    return contents;
}
