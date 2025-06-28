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
 * @brief Checks if a socket fd is ready for writing
 *
 * This function checks if the specified socket fd is ready to write data by
 * polling the socket file descriptor. It returns SUCCESS if the fd is
 * ready, or FAILURE if not.
 *
 * @param sockfd The socket file descriptor to check
 * @return SUCCESS if the fd is ready for writing, FAILURE otherwise
 */
static bool check_writing(int sockfd)
{
    struct pollfd pfd;

    pfd.fd = sockfd;
    pfd.events = POLLOUT;
    pfd.revents = 0;
    if (poll(&pfd, 1, 0) <= 0 || !(pfd.revents & POLLOUT)) {
        return FAILURE;
    }
    return SUCCESS;
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
void write_to_client(client_t *client, char *message, int fallback_sockfd)
{
    if (client == NULL && fallback_sockfd >= 0 &&
        check_writing(fallback_sockfd)) {
        write(fallback_sockfd, message, strlen(message));
        return;
    }
    if (!client || client->sockfd < 0) {
        puts("Client is NULL or has invalid sockfd");
        if (client->server != NULL) {
            debug_warning(client->server->options->debug,
                "Attempted to send message to invalid client");
        }
        return;
    }
    if (!check_writing(client->sockfd)) {
        debug_warning(client->server->options->debug,
            "Client %d is not ready for writing", client->index);
        return;
    }
    write(client->sockfd, message, strlen(message));
}

/**
 * @brief Formats a message using variadic arguments
 *
 * This function takes a format string and variadic arguments and formats
 * them into a buffer.
 *
 * @param buffer Buffer to store the formatted message
 * @param buffer_size Size of the buffer
 * @param format Format string for the message
 * @param args Variadic arguments list
 * @return Length of the formatted string, or -1 on error
 */
static int format_message(
    char *buffer, size_t buffer_size, const char *format, va_list args)
{
    int len = vsnprintf(buffer, buffer_size, format, args);

    if (len <= 0 || len >= (int)buffer_size) {
        return -1;
    }
    buffer[len] = '\0';
    return len;
}

/**
 * @brief Allocates and copies a message string
 *
 * This function allocates memory for a message and copies the content
 * from the source buffer.
 *
 * @param client Pointer to the client structure (for error reporting)
 * @param buffer Source buffer containing the message
 * @param len Length of the message
 * @return Pointer to the allocated message, or NULL on failure
 */
static char *allocate_message(client_t *client, const char *buffer, int len)
{
    char *message = malloc(len + 1);

    if (message == NULL) {
        debug_warning(client->server->options->debug,
            "Failed to allocate memory for message to client %d",
            client->index);
        return NULL;
    }
    strcpy(message, buffer);
    return message;
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
void send_to_client(client_t *client, const char *format, ...)
{
    va_list args;
    char buffer[4096];
    int len;
    char *message;

    if (client == NULL || format == NULL)
        return;
    va_start(args, format);
    len = format_message(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (len == -1) {
        debug_warning(client->server->options->debug,
            "Failed to format message for client %d", client->index);
        return;
    }
    message = allocate_message(client, buffer, len);
    if (message == NULL)
        return;
    add_message_to_buffer(client, message);
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
            send_to_client(server->clients[i], "%s", buffer);
        }
    }
}
