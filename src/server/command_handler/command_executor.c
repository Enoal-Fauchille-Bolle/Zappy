/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Executor
*/

#include "command_handler/command_executor.h"
#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "debug.h"
#include <stdbool.h>
#include <strings.h>
#include <unistd.h>

const command_registry_entry_t ai_command_registry[] = {
    {"forward", forward_command}, {"left", left_command},
    {"right", right_command}, {"look", look_command},
    {"inventory", inventory_command}, {"connect_nbr", connect_nbr_command},
    {"fork", fork_command}, {"take", take_command}, {"set", set_command},
    {"eject", eject_command}, {"broadcast", broadcast_command},
    {"incantation", incantation_command}, {NULL, NULL}};

const command_registry_entry_t gui_command_registry[] = {{"msz", msz_command},
    {"ppo", ppo_command}, {"bct", bct_command}, {"mct", mct_command},
    {"plv", plv_command}, {"tna", tna_command}, {"pin", pin_command},
    {"sgt", sgt_command}, {"sst", sst_command}, {NULL, NULL}};

/**
 * @brief Get the command handler for a given command.
 *
 * This function searches for the command handler corresponding to the
 * provided command name in the command_handlers array.
 *
 * @param command The command structure containing the command name.
 * @return command_handler_t The handler for the specified command.
 */
static command_registry_entry_t get_command_registry_entry(
    const command_registry_entry_t *command_registry_entry, command_t *command)
{
    for (int i = 0; command_registry_entry[i].command_name; i++) {
        if (strcasecmp(
                command_registry_entry[i].command_name, command->name) == 0) {
            return command_registry_entry[i];
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
 */
void execute_command(client_t *client, command_t *command)
{
    command_registry_entry_t handler = {0};

    if (!client)
        return;
    if (!client->is_gui) {
        handler = get_command_registry_entry(ai_command_registry, command);
    } else {
        handler = get_command_registry_entry(gui_command_registry, command);
    }
    if (!handler.handler) {
        debug_warning(client->server->options->debug,
            "Invalid command: '%s'\n", command->name);
        if (!client->is_gui) {
            send_to_client(client, "ko\n");
        } else {
            send_to_client(client, "suc\n");
        }
        return;
    }
    handler.handler(client, command);
}
