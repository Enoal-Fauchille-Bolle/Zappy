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

typedef struct map_s map_t;

typedef struct inventory_s {
    size_t food;
    size_t linemate;
    size_t deraumere;
    size_t sibur;
    size_t mendiane;
    size_t phiras;
    size_t thystame;
} inventory_t, ressources_t;

size_t get_ressource_count(const map_t *map, const float density);

#endif /* !RESSOURCES_H_ */
