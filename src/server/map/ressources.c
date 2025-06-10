/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ressources
*/

#include "map/ressources.h"
#include "map/map.h"
#include <stdio.h>
#include <stdlib.h>

size_t get_minimum_ressource_count(const map_t *map, const float density)
{
    if (map == NULL) {
        fprintf(stderr, "Invalid map pointer\n");
        return 0;
    }
    return map->width * map->height * density;
}

// static void spread_ressource(map_t *map, const ressource_t ressource)
// {
//     size_t count = get_ressource_count(map, ressource_densities[ressource]);
//     size_t x, y;

//     for (size_t i = 0; i < count; i++) {
//         x = rand() % map->width;
//         y = rand() % map->height;
//         tile_t *tile = get_tile(map, (pos_t){x, y});

//         if (tile == NULL) {
//             fprintf(stderr, "Failed to get tile at (%zu, %zu)\n", x, y);
//             continue;
//         }
//         switch (ressource) {
//             case FOOD:
//                 tile->ressources.food++;
//                 break;
//             case LINEMATE:
//                 tile->ressources.linemate++;
//                 break;
//             case DERAUMERE:
//                 tile->ressources.deraumere++;
//                 break;
//             case SIBUR:
//                 tile->ressources.sibur++;
//                 break;
//             case MENDIANE:
//                 tile->ressources.mendiane++;
//                 break;
//             case PHIRAS:
//                 tile->ressources.phiras++;
//                 break;
//             case THYSTAME:
//                 tile->ressources.thystame++;
//                 break;
//             default:
//                 fprintf(stderr, "Unknown resource type\n");
//         }
//     }
// }
