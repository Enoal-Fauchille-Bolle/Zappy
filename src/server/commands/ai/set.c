/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Set Command
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug_categories.h"
#include "map/resource_names.h"
#include "map/resources.h"
#include "team/player/player.h"
#include <stdbool.h>
#include <unistd.h>

/**
 * @brief Check if the command arguments are valid.
 *
 * This function checks if the command has the required number of arguments.
 * If not, it sends an error message to the client and logs the invalid
 * attempt.
 *
 * @param client The client that sent the command.
 * @param command The command structure containing the arguments.
 * @return SUCCESS if the arguments are valid, FAILURE otherwise.
 */
static bool check_args(client_t *client, command_t *command)
{
    if (command->argc < 1) {
        write(client->sockfd, "ko\n", 3);
        debug_cmd(client->server->options->debug,
            "Player %zu sent an invalid take command arguments\n",
            client->player->id);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Check if the resource is valid.
 *
 * This function checks if the resource specified in the command is valid.
 * If the resource is invalid, it sends an error message to the client and
 * logs the attempt.
 *
 * @param client The client that sent the command.
 * @param command The command structure containing the resource name.
 * @param resource The resource type to check.
 * @return SUCCESS if the resource is valid, FAILURE otherwise.
 */
static bool check_resource_validity(
    client_t *client, command_t *command, resource_t resource)
{
    if (resource == RESOURCE_COUNT) {
        write(client->sockfd, "ko\n", 3);
        debug_cmd(client->server->options->debug,
            "Player %zu tried to take an invalid resource: %s\n",
            client->player->id, command->argv[0]);
        return FAILURE;
    }
    return SUCCESS;
}

void set_command(client_t *client, command_t *command)
{
    resource_t resource = RESOURCE_COUNT;

    if (check_args(client, command) == FAILURE)
        return;
    resource = get_resource_from_string(command->argv[0]);
    if (check_resource_validity(client, command, resource) == FAILURE)
        return;
    if (!set_resource(client->player, client->server->game->map, resource)) {
        write(client->sockfd, "ko\n", 3);
        debug_map(client->server->options->debug,
            "Player %zu failed to take resource: %s\n", client->player->id,
            resource_names[resource]);
        return;
    }
    client->player->tick_cooldown = SET_COMMAND_COOLDOWN;
    write(client->sockfd, "ok\n", 3);
    debug_map(client->server->options->debug,
        "Player %zu took '%s' at (%d, %d)\n", client->player->id,
        resource_names[resource], client->player->pos.x,
        client->player->pos.y);
}
