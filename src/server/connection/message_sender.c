/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Message Sender
*/

#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "constants.h"
#include "debug.h"
#include <poll.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <unistd.h>

/**
 * @brief Checks if a client is ready for writing
 *
 * This function checks if the specified client is ready to write data by
 * polling the socket file descriptor. It returns SUCCESS if the client is
 * ready, or FAILURE if not.
 *
 * @param client Pointer to the client structure
 * @return SUCCESS if the client is ready for writing, FAILURE otherwise
 */
static bool check_writing(client_t *client)
{
    struct pollfd pfd;

    pfd.fd = client->sockfd;
    pfd.events = POLLOUT;
    pfd.revents = 0;
    if (poll(&pfd, 1, 0) <= 0 || !(pfd.revents & POLLOUT)) {
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Validates if a client is ready to receive messages
 *
 * This function checks if the client is valid and ready for writing
 * operations.
 *
 * @param client Pointer to the client structure
 * @return FAILURE if client is valid and ready, SUCCESS otherwise
 */
static bool validate_client_for_sending(client_t *client)
{
    if (client == NULL || client->sockfd < 0) {
        if (client != NULL && client->server != NULL) {
            debug_warning(client->server->options->debug,
                "Attempted to send message to invalid client");
        }
        return FAILURE;
    }
    if (!check_writing(client)) {
        debug_warning(client->server->options->debug,
            "Client %d is not ready for writing", client->index);
        return FAILURE;
    }
    return SUCCESS;
}

/**
 * @brief Formats a message and adds it to the client's writing buffer
 *
 * This function formats the message using the provided format string and
 * arguments, then adds it to the client's writing buffer for later sending.
 *
 * @param client Pointer to the client structure
 * @param format Format string for the message
 * @param ... Additional arguments for formatting
 */
void queue_message_to_client(client_t *client, const char *format, ...)
{
    va_list args;
    char buffer[4096];
    int len;
    char *message;

    if (client == NULL || format == NULL) {
        return;
    }
    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (len <= 0 || len >= (int)sizeof(buffer)) {
        debug_warning(client->server->options->debug,
            "Failed to format message for client %d", client->index);
        return;
    }
    buffer[len] = '\0'; // Ensure null termination
    message = malloc(len + 1);
    if (message == NULL) {
        debug_warning(client->server->options->debug,
            "Failed to allocate memory for message to client %d", client->index);
        return;
    }
    strcpy(message, buffer);
    add_message_to_buffer(client, message);
}

/**
 * @brief Sends a pre-formatted message directly to a client
 *
 * This function sends a pre-formatted message directly to the specified client
 * if the client is valid and ready for writing.
 *
 * @param client Pointer to the client structure
 * @param message Pre-formatted message string to send
 */
void send_to_client(client_t *client, char *message)
{
    if (!validate_client_for_sending(client) || message == NULL) {
        return;
    }
    write(client->sockfd, message, strlen(message));
}

/**
 * @brief Queues a formatted message to all GUI clients
 *
 * This function formats a message and adds it to the writing buffer of all
 * clients that are marked as GUI clients.
 *
 * @param server Pointer to the server structure
 * @param format Format string for the message
 * @param ... Additional arguments for formatting
 */
void send_to_all_guis(server_t *server, const char *format, ...)
{
    va_list args;
    char buffer[4096];
    int len;

    if (server == NULL) {
        return;
    }
    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (len <= 0 || len >= (int)sizeof(buffer)) {
        return;
    }
    buffer[len] = '\0';
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (server->clients[i] != NULL && server->clients[i]->is_gui) {
            queue_message_to_client(server->clients[i], "%s", buffer);
        }
    }
}
