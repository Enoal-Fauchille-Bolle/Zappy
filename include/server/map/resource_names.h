/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** resource_names
*/
// clang-format Language: C

#ifndef RESOURCE_NAMES_H_
    #define RESOURCE_NAMES_H_

    #include "map/resources.h"

static const char *resource_names[7] = {"food", "linemate", "deraumere",
    "sibur", "mendiane", "phiras", "thystame"};

resource_t get_resource_from_string(char *resource_name);

#endif /* !RESOURCE_NAMES_H_ */
