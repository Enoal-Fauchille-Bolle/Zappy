/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ressources
*/
// clang-format Language: C

#ifndef RESSOURCES_H_
    #define RESSOURCES_H_
    #include <stddef.h>

    #define FOOD_DENSITY 0.5f
    #define LINEMATE_DENSITY 0.3f
    #define DERAUMERE_DENSITY 0.15f
    #define SIBUR_DENSITY 0.1f
    #define MENDIANE_DENSITY 0.1f
    #define PHIRAS_DENSITY 0.08f
    #define THYSTAME_DENSITY 0.05f

typedef enum ressource_e {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
} ressource_t;

static const float ressource_densities[7] = {FOOD_DENSITY, LINEMATE_DENSITY,
    DERAUMERE_DENSITY, SIBUR_DENSITY, MENDIANE_DENSITY, PHIRAS_DENSITY,
    THYSTAME_DENSITY};

typedef struct map_s map_t;

typedef size_t ressources_t[7];
typedef ressources_t inventory_t;

size_t get_minimum_ressource_count(const map_t *map, const float density);
// void spread_ressource(map_t *map, const ressource_t ressource);
void spread_ressources(map_t *map);

#endif /* !RESSOURCES_H_ */
