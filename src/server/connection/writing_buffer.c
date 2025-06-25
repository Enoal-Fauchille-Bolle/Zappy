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
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Shifts all messages in the buffer to the left starting from a given
 * index
 *
 * This function moves all messages one position to the left in the client's
 * writing buffer, starting from the specified index. The last position is set
 * to NULL.
 *
 * @param client Pointer to the client structure containing the writing buffer
 * @param start_index The index from which to start shifting messages left
 */
static void shift_messages_left(client_t *client, int start_index)
{
    for (int i = start_index; i < MAX_WRITING_BUFFER_SIZE - 1; i++) {
        client->writing_buffer[i] = client->writing_buffer[i + 1];
        client->writing_buffer[i + 1] = NULL;
    }
}

/**
 * @brief Removes the first message from the client's writing buffer
 *
 * This function checks if the client pointer is valid, then iterates through
 * the writing buffer to find and remove the first non-NULL message. After
 * removing the message, it shifts all remaining messages to the left to
 * maintain queue order.
 *
 * @param client Pointer to the client structure
 * @return char* Pointer to the removed message, or NULL if buffer is empty
 */
char *pop_message_from_buffer(client_t *client)
{
    char *message = NULL;

    if (client == NULL) {
        fprintf(stderr, "Invalid client pointer\n");
        return NULL;
    }
    for (int i = 0; i < MAX_WRITING_BUFFER_SIZE; i++) {
        if (client->writing_buffer[i] != NULL) {
            message = client->writing_buffer[i];
            client->writing_buffer[i] = NULL;
            shift_messages_left(client, i);
            return message;
        }
    }
    return NULL;
}

/**
 * @brief Adds a message to the client's writing buffer
 *
 * This function checks if the client and message pointers are valid, then
 * attempts to add the message to the client's writing buffer at the first
 * available position (FIFO queue - add to end). If the buffer is full, it
 * logs a warning message and frees the message to prevent memory leaks.
 *
 * @param client Pointer to the client structure
 * @param message Pointer to the message string to be added
 */
void add_message_to_buffer(client_t *client, char *message)
{
    if (client == NULL || message == NULL) {
        fprintf(stderr, "Invalid client or message pointer\n");
        if (message != NULL)
            free(message);
        return;
    }
    for (int i = 0; i < MAX_WRITING_BUFFER_SIZE; i++) {
        if (client->writing_buffer[i] == NULL) {
            client->writing_buffer[i] = message;
            return;
        }
    }
    debug_warning(client->server->options->debug,
        "Writing buffer is full for client %d\n", client->index);
    free(message);
}

/**
 * @brief Checks if the client's writing buffer is empty
 *
 * This function iterates through the writing buffer to check if there are
 * any pending messages to be sent.
 *
 * @param client Pointer to the client structure
 * @return true if the buffer is empty, false otherwise
 */
static bool is_writing_buffer_empty(client_t *client)
{
    if (client == NULL) {
        return true;
    }
    for (int i = 0; i < MAX_WRITING_BUFFER_SIZE; i++) {
        if (client->writing_buffer[i] != NULL) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Handle writing buffer for a single client
 *
 * This function checks if the client's writing buffer is empty. If not, it
 * pops a message from the buffer and writes it to the client. It then calls
 * itself recursively to handle any remaining messages in the buffer.
 *
 * @param client Pointer to the client structure to handle writing buffer for
 */
static void handle_client_writing_buffer(client_t *client)
{
    char *message = NULL;

    if (client == NULL || is_writing_buffer_empty(client)) {
        return;
    }
    message = pop_message_from_buffer(client);
    if (message != NULL) {
        write_to_client(client, message);
        free(message);
    }
    handle_client_writing_buffer(client);
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
 * This function frees all allocated messages in the writing buffer and
 * sets all positions to NULL.
 *
 * @param client Pointer to the client structure
 */
void clear_writing_buffer(client_t *client)
{
    if (client == NULL) {
        return;
    }
    for (int i = 0; i < MAX_WRITING_BUFFER_SIZE; i++) {
        if (client->writing_buffer[i] != NULL) {
            free(client->writing_buffer[i]);
            client->writing_buffer[i] = NULL;
        }
    }
}
