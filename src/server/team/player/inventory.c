/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Inventory
*/

#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "map/resources.h"
#include "map/tile.h"
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
    return inventory_str;
}

/**
 * @brief Take a resource from the tile where the player is located.
 *
 * This function allows a player to take a specified resource from the tile
 * they are currently on. If the resource is available, it is removed from
 * the tile and added to the player's inventory.
 *
 * @param player Pointer to the player structure.
 * @param map Pointer to the map structure.
 * @param resource The type of resource to take (e.g., FOOD, LINEMATE, etc.).
 * @return true if the resource was successfully taken, false otherwise.
 */
bool take_resource(player_t *player, map_t *map, resource_t resource)
{
    tile_t *tile;

    if (!player || !map || resource < FOOD || resource >= RESOURCE_COUNT) {
        fprintf(stderr, "Invalid parameters for taking resource\n");
        return false;
    }
    tile = get_tile(map, player->pos);
    if (!tile) {
        fprintf(stderr, "Tile not found for player position\n");
        return false;
    }
    if (tile->resources[resource] > 0) {
        player->inventory[resource]++;
        tile->resources[resource]--;
        return true;
    }
    return false;
}

/**
 * @brief Set a resource on the tile where the player is located.
 *
 * This function allows a player to set a specified resource on the tile
 * they are currently on. If the player has the resource in their inventory,
 * it is removed from the inventory and added to the tile's resources.
 *
 * @param player Pointer to the player structure.
 * @param map Pointer to the map structure.
 * @param resource The type of resource to set (e.g., FOOD, LINEMATE, etc.).
 * @return true if the resource was successfully set, false otherwise.
 */
bool set_resource(player_t *player, map_t *map, resource_t resource)
{
    tile_t *tile;

    if (!player || !map || resource < FOOD || resource >= RESOURCE_COUNT) {
        fprintf(stderr, "Invalid parameters for setting resource\n");
        return false;
    }
    tile = get_tile(map, player->pos);
    if (!tile) {
        fprintf(stderr, "Tile not found for player position\n");
        return false;
    }
    if (player->inventory[resource] > 0) {
        player->inventory[resource]--;
        tile->resources[resource]++;
        pdr_command(player, resource);
        return true;
    }
    return false;
}
