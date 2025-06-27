/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Buffer Management
*/

#include "command_handler/command_buffer.h"
#include "command_handler/command.h"
#include "command_handler/command_factory.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "vector.h"
#include <stdio.h>

/**
 * @brief Removes the first command from the client's command buffer
 *
 * This function checks if the client pointer is valid, then removes the first
 * command from the vector-based command buffer. Maintains FIFO queue order.
 *
 * @param client Pointer to the client structure
 * @return command_t* Pointer to the removed command, or NULL if buffer is
 * empty
 */
command_t *pop_command_from_buffer(client_t *client)
{
    const vector_vtable_t *vtable;
    command_t *command = NULL;
    command_t **command_ptr;

    if (client == NULL || client->command_buffer == NULL) {
        fprintf(stderr, "Invalid client pointer or command buffer\n");
        return NULL;
    }
    vtable = vector_get_vtable(client->command_buffer);
    if (vtable->size(client->command_buffer) == 0) {
        return NULL;
    }
    command_ptr = (command_t **)vtable->at(client->command_buffer, 0);
    if (command_ptr == NULL) {
        return NULL;
    }
    command = *command_ptr;
    vtable->erase(client->command_buffer, 0);
    return command;
}

/**
 * @brief Validates client and command parameters for buffer operations
 *
 * This function checks if the client and command pointers are valid
 * and handles cleanup if validation fails.
 *
 * @param client Pointer to the client structure
 * @param command Pointer to the command structure
 * @return true if parameters are valid, false otherwise
 */
static bool validate_command_buffer_parameters(
    client_t *client, command_t *command)
{
    if (client == NULL || command == NULL) {
        fprintf(stderr, "Invalid client or command pointer\n");
        destroy_command(command);
        return false;
    }
    if (client->command_buffer == NULL) {
        fprintf(stderr, "Command buffer not initialized for client %d\n",
            client->index);
        destroy_command(command);
        return false;
    }
    return true;
}

/**
 * @brief Checks if the command buffer has space for new commands
 *
 * This function checks if the client's command buffer can accept new commands.
 * For AI clients, it enforces the MAX_COMMAND_BUFFER_SIZE limit.
 * GUI clients have no limit.
 *
 * @param client Pointer to the client structure
 * @param vtable Pointer to the vector vtable
 * @return true if buffer has space, false otherwise
 */
static bool check_command_buffer_capacity(
    client_t *client, const vector_vtable_t *vtable)
{
    if (!client->is_gui &&
        vtable->size(client->command_buffer) >= MAX_COMMAND_BUFFER_SIZE) {
        debug_warning(client->server->options->debug,
            "Command buffer is full for AI client %d\n", client->index);
        return false;
    }
    return true;
}

/**
 * @brief Adds a command to the client's command buffer
 *
 * This function checks if the client and command pointers are valid, then
 * adds the command to the client's vector-based command buffer. For AI
 * clients, it enforces the MAX_COMMAND_BUFFER_SIZE limit. GUI clients have no
 * limit.
 *
 * @param client Pointer to the client structure
 * @param command Pointer to the command structure to be added
 */
void add_command_to_buffer(client_t *client, command_t *command)
{
    const vector_vtable_t *vtable;

    if (!validate_command_buffer_parameters(client, command)) {
        return;
    }
    vtable = vector_get_vtable(client->command_buffer);
    if (!check_command_buffer_capacity(client, vtable)) {
        destroy_command(command);
        return;
    }
    vtable->push_back(client->command_buffer, &command);
}

/**
 * @brief Checks if the client's command buffer is empty
 *
 * This function checks if the vector-based command buffer contains any
 * pending commands to be processed.
 *
 * @param client Pointer to the client structure
 * @return true if the buffer is empty, false otherwise
 */
bool is_command_buffer_empty(client_t *client)
{
    const vector_vtable_t *vtable;

    if (client == NULL || client->command_buffer == NULL) {
        return true;
    }
    vtable = vector_get_vtable(client->command_buffer);
    return vtable->size(client->command_buffer) == 0;
}

/**
 * @brief Clears all commands from the client's command buffer
 *
 * This function destroys all commands in the vector-based command
 * buffer and clears the vector.
 *
 * @param client Pointer to the client structure
 */
void clear_command_buffer(client_t *client)
{
    const vector_vtable_t *vtable;
    command_t **command_ptr = NULL;
    size_t size = 0;

    if (client == NULL || client->command_buffer == NULL) {
        return;
    }
    vtable = vector_get_vtable(client->command_buffer);
    size = vtable->size(client->command_buffer);
    for (size_t i = 0; i < size; i++) {
        command_ptr = (command_t **)vtable->at(client->command_buffer, i);
        if (command_ptr != NULL && *command_ptr != NULL) {
            destroy_command(*command_ptr);
        }
    }
    vtable->clear(client->command_buffer);
}
