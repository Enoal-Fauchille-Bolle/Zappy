/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Writing Buffer
*/

#include "connection/writing_buffer.h"
#include "connection/client.h"
#include "connection/message_sender.h"
#include "connection/server.h"
#include "constants.h"
#include "debug.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Removes the first message from the client's writing buffer
 *
 * This function checks if the client pointer is valid, then removes the first
 * message from the vector-based writing buffer. Maintains FIFO queue order.
 *
 * @param client Pointer to the client structure
 * @return char* Pointer to the removed message, or NULL if buffer is empty
 */
char *pop_message_from_buffer(client_t *client)
{
    const vector_vtable_t *vtable;
    char *message = NULL;
    char **message_ptr;

    if (client == NULL || client->writing_buffer == NULL) {
        fprintf(stderr, "Invalid client pointer or writing buffer\n");
        return NULL;
    }
    vtable = vector_get_vtable(client->writing_buffer);
    if (vtable->size(client->writing_buffer) == 0) {
        return NULL;
    }
    message_ptr = (char **)vtable->at(client->writing_buffer, 0);
    if (message_ptr == NULL) {
        return NULL;
    }
    message = *message_ptr;
    vtable->erase(client->writing_buffer, 0);
    return message;
}

/**
 * @brief Validates client and message parameters for buffer operations
 *
 * This function checks if the client and message pointers are valid
 * and handles cleanup if validation fails.
 *
 * @param client Pointer to the client structure
 * @param message Pointer to the message string
 * @return true if parameters are valid, false otherwise
 */
static bool validate_buffer_parameters(client_t *client, char *message)
{
    if (client == NULL || message == NULL) {
        fprintf(stderr, "Invalid client or message pointer\n");
        if (message != NULL)
            free(message);
        return false;
    }
    if (client->writing_buffer == NULL) {
        fprintf(stderr, "Writing buffer not initialized for client %d\n",
            client->index);
        free(message);
        return false;
    }
    return true;
}

/**
 * @brief Checks if the writing buffer has space for new messages
 *
 * This function checks if the client's writing buffer can accept new messages.
 * For AI clients, it enforces the MAX_WRITING_BUFFER_SIZE limit.
 * GUI clients have no limit.
 *
 * @param client Pointer to the client structure
 * @param vtable Pointer to the vector vtable
 * @param message Pointer to the message (for cleanup on failure)
 * @return true if buffer has space, false otherwise
 */
static bool check_buffer_capacity(
    client_t *client, const vector_vtable_t *vtable, char *message)
{
    if (!client->is_gui &&
        vtable->size(client->writing_buffer) >= MAX_WRITING_BUFFER_SIZE) {
        debug_warning(client->server->options->debug,
            "Writing buffer is full for AI client %d\n", client->index);
        free(message);
        return false;
    }
    return true;
}

/**
 * @brief Adds a message to the client's writing buffer
 *
 * This function checks if the client and message pointers are valid, then
 * adds the message to the client's vector-based writing buffer. For AI
 * clients, it enforces the MAX_WRITING_BUFFER_SIZE limit. GUI clients have no
 * limit.
 *
 * @param client Pointer to the client structure
 * @param message Pointer to the message string to be added
 */
void add_message_to_buffer(client_t *client, char *message)
{
    const vector_vtable_t *vtable;

    if (!validate_buffer_parameters(client, message)) {
        return;
    }
    vtable = vector_get_vtable(client->writing_buffer);
    if (!check_buffer_capacity(client, vtable, message)) {
        return;
    }
    vtable->push_back(client->writing_buffer, &message);
}

/**
 * @brief Checks if the client's writing buffer is empty
 *
 * This function checks if the vector-based writing buffer contains any
 * pending messages to be sent.
 *
 * @param client Pointer to the client structure
 * @return true if the buffer is empty, false otherwise
 */
static bool is_writing_buffer_empty(client_t *client)
{
    const vector_vtable_t *vtable;

    if (client == NULL || client->writing_buffer == NULL) {
        return true;
    }
    vtable = vector_get_vtable(client->writing_buffer);
    return vtable->size(client->writing_buffer) == 0;
}

/**
 * @brief Handle writing buffer for a single client
 *
 * This function checks if the client's writing buffer is empty. If not, it
 * pops messages from the buffer and writes them to the client in a loop until
 * the buffer is empty.
 *
 * @param client Pointer to the client structure to handle writing buffer for
 */
static void handle_client_writing_buffer(client_t *client)
{
    char *message = NULL;

    if (client == NULL) {
        return;
    }
    while (!is_writing_buffer_empty(client)) {
        message = pop_message_from_buffer(client);
        if (message != NULL) {
            write_to_client(client, message);
            free(message);
        } else {
            break;
        }
    }
}

/**
 * @brief Handle writing buffers for all clients in the server
 *
 * This function iterates through all clients in the server and checks if their
 * writing buffer is not empty. If it is not empty, it calls the function to
 * handle the client's writing buffer.
 *
 * @param server Pointer to the server structure containing clients
 */
void handle_clients_writing_buffer(server_t *server)
{
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] == NULL ||
            is_writing_buffer_empty(server->clients[i]))
            continue;
        handle_client_writing_buffer(server->clients[i]);
    }
}

/**
 * @brief Clears all messages from the client's writing buffer
 *
 * This function frees all allocated messages in the vector-based writing
 * buffer and clears the vector.
 *
 * @param client Pointer to the client structure
 */
void clear_writing_buffer(client_t *client)
{
    const vector_vtable_t *vtable;
    char **message_ptr = NULL;
    size_t size = 0;

    if (client == NULL || client->writing_buffer == NULL) {
        return;
    }
    vtable = vector_get_vtable(client->writing_buffer);
    size = vtable->size(client->writing_buffer);
    for (size_t i = 0; i < size; i++) {
        message_ptr = (char **)vtable->at(client->writing_buffer, i);
        if (message_ptr != NULL && *message_ptr != NULL) {
            free(*message_ptr);
        }
    }
    vtable->clear(client->writing_buffer);
}
