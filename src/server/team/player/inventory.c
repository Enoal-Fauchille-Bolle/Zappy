/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Inventory
*/

#include "connection/client.h"
#include "map/resources.h"
#include "team/player/player.h"
#include <stdio.h>

/**
 * @brief Check the player's inventory and return its contents as a string.
 *
 * This function retrieves the player's inventory and formats it into a
 * string representation. The string includes the counts of each resource in
 * the player's inventory, and it is dynamically allocated.
 *
 * @param player Pointer to the player structure whose inventory is being
 * checked.
 * @return A dynamically allocated string containing the inventory contents,
 *         or NULL if an error occurs.
 */
char *check_inventory(player_t *player)
{
    char *inventory_str;

    if (player == NULL) {
        fprintf(stderr, "Invalid player pointer\n");
        return NULL;
    }
    inventory_str = get_inventory_string(player->inventory);
    if (inventory_str == NULL) {
        fprintf(stderr, "Failed to get inventory string\n");
        return NULL;
    }
    player->tick_cooldown = 1;
    return inventory_str;
}
