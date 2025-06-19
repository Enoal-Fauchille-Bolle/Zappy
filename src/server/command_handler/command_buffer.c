/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Buffer Management
*/

#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include <stdio.h>

/**
 * @brief Shifts all commands in the buffer to the left starting from a given
 * index
 *
 * This function moves all commands one position to the left in the client's
 * command buffer, starting from the specified index. The last position is set
 * to NULL.
 *
 * @param client Pointer to the client structure containing the command buffer
 * @param start_index The index from which to start shifting commands left
 */
static void shift_commands_left(client_t *client, int start_index)
{
    for (int i = start_index; i < MAX_COMMAND_BUFFER_SIZE - 1; i++) {
        client->command[i] = client->command[i + 1];
        client->command[i + 1] = NULL;
    }
}

/**
 * @brief Removes the first command from the client's command buffer
 *
 * This function checks if the client pointer is valid, then iterates through
 * the command buffer to find and remove the first non-NULL command. After
 * removing the command, it shifts all remaining commands to the left to
 * maintain queue order.
 *
 * @param client Pointer to the client structure
 */
command_t *pop_command_from_buffer(client_t *client)
{
    command_t *command = NULL;

    if (client == NULL) {
        fprintf(stderr, "Invalid client pointer\n");
        return NULL;
    }
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        if (client->command[i] != NULL) {
            command = client->command[i];
            client->command[i] = NULL;
            shift_commands_left(client, i);
            return command;
        }
    }
    return NULL;
}

/**
 * @brief Adds a command to the client's command buffer
 *
 * This function checks if the client and command pointers are valid, then
 * attempts to add the command to the client's command buffer at the first
 * available position (FIFO queue - add to end). If the buffer is full, it
 * logs a warning message.
 *
 * @param client Pointer to the client structure
 * @param command Pointer to the command structure to be added
 */
void add_command_to_buffer(client_t *client, command_t *command)
{
    if (client == NULL || command == NULL) {
        fprintf(stderr, "Invalid client or command pointer\n");
        return;
    }
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        if (client->command[i] == NULL) {
            client->command[i] = command;
            return;
        }
    }
    debug_warning(client->server->options->debug,
        "Command buffer is full for client %d\n", client->index);
}
