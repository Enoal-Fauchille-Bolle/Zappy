/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** resources
*/
// clang-format Language: C

#ifndef RESOURCES_H_
    #define RESOURCES_H_
    #include <stddef.h>

    #define FOOD_DENSITY 0.5f
    #define LINEMATE_DENSITY 0.3f
    #define DERAUMERE_DENSITY 0.15f
    #define SIBUR_DENSITY 0.1f
    #define MENDIANE_DENSITY 0.1f
    #define PHIRAS_DENSITY 0.08f
    #define THYSTAME_DENSITY 0.05f

typedef enum resource_e {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
} resource_t;

static const float resource_densities[7] = {FOOD_DENSITY, LINEMATE_DENSITY,
    DERAUMERE_DENSITY, SIBUR_DENSITY, MENDIANE_DENSITY, PHIRAS_DENSITY,
    THYSTAME_DENSITY};

typedef struct map_s map_t;

typedef size_t resources_t[7];
typedef resources_t inventory_t;

size_t get_minimum_resource_count(const map_t *map, const float density);
size_t count_resource(const map_t *map, const resource_t resource);
void spread_resource(map_t *map, const resource_t resource);
void spread_resources(map_t *map);

#endif /* !RESOURCES_H_ */
