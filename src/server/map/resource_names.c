/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource Names Management
*/

#include "map/resource_names.h"
#include "map/resources.h"
#include <strings.h>

/**
 * @brief Get the resource type from a command string.
 *
 * This function maps a command string to its corresponding resource type.
 * If the command does not match any known resource, it returns RESOURCE_COUNT.
 *
 * @param command The command string to check
 * @return The resource type corresponding to the command, or RESOURCE_COUNT
 *         if not found.
 */
resource_t get_resource_from_string(char *resource_name)
{
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        if (strcasecmp(resource_name, resource_names[i]) == 0) {
            return i;
        }
    }
    return RESOURCE_COUNT;
}
