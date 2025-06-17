/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Executor
*/

#include "command_handler/command_executor.h"
#include "command_handler/command.h"
#include "command_handler/command_status.h"
#include "command_handler/commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "debug.h"
#include <stdbool.h>
#include <strings.h>
#include <unistd.h>

const command_registry_entry_t command_registry[] = {
    {"dummy", dummy_command}, {NULL, NULL}};

/**
 * @brief Get the command handler for a given command.
 *
 * This function searches for the command handler corresponding to the
 * provided command name in the command_handlers array.
 *
 * @param command The command structure containing the command name.
 * @return command_handler_t The handler for the specified command.
 */
static command_registry_entry_t get_command_registry_entry(command_t *command)
{
    for (int i = 0; command_registry[i].command_name; i++) {
        if (strcasecmp(command_registry[i].command_name, command->name) == 0) {
            return command_registry[i];
        }
    }
    return (command_registry_entry_t){NULL, NULL};
}

/**
 * @brief Execute a command for a client.
 *
 * This function retrieves the appropriate command handler based on the
 * command name and executes it, passing the command and client as
 * arguments.
 *
 * @param command The command structure containing the command name and
 * arguments.
 * @param client The client structure associated with the command.
 * @return command_status_t The status of the command execution.
 */
command_status_t execute_command(client_t *client, command_t *command)
{
    command_registry_entry_t handler = {0};

    handler = get_command_registry_entry(command);
    if (!handler.handler) {
        debug_warning(client->server->options->debug,
            "Invalid command: '%s'\n", command->name);
        write(client->sockfd, "ko\n", 3);
        return COMMAND_NOT_FOUND;
    }
    return handler.handler(client, command);
}
