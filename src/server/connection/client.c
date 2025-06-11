/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Management
*/

#include "connection/client.h"
#include "connection/socket.h"
#include "connection/connection_handler.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Destroys a client structure and releases all associated resources
 *
 * This function safely destroys a client by closing its socket file
 * descriptor, closing its associated stream, freeing its address structure,
 * and finally freeing the client structure itself. The function performs null
 * checks to ensure safe cleanup.
 *
 * @param client Pointer to the client structure to destroy. Can be NULL.
 */
void destroy_client(client_t *client)
{
    if (!client)
        return;
    if (client->sockfd >= 0)
        close(client->sockfd);
    if (client->stream)
        fclose(client->stream);
    if (client->addr)
        free(client->addr);
    free(client);
}

void process_client_message(
    client_t *client, struct pollfd *fds, int client_index)
{
    char *message = read_socket(client->sockfd);

    if (!message) {
        remove_client(client->server, fds, client_index);
        return;
    }
    printf("Received message from client %d: '%s'\n", client->sockfd, message);
    free(message);
    return;
}

/**
 * @brief Initializes a new client structure and establishes connection
 *
 * Creates and configures a client structure with the provided socket file
 * descriptor and address information. Sets up a file stream for reading from
 * the client socket and sends a welcome message.
 *
 * @param server Pointer to the server structure
 * @param client_sockfd Socket file descriptor for the client connection
 * @param client_addr Pointer to the client's socket address structure
 *
 * @return Pointer to the initialized client structure on success, NULL on
 * failure
 *
 * @note On failure, the function automatically cleans up allocated resources
 * @warning The caller is responsible for freeing the returned client structure
 */
client_t *init_client(
    server_t *server, int client_sockfd, struct sockaddr_in *client_addr)
{
    client_t *client = malloc(sizeof(client_t));

    client->server = server;
    client->sockfd = client_sockfd;
    client->addr = malloc(sizeof(struct sockaddr_in));
    if (!client->addr) {
        perror("malloc");
        destroy_client(client);
        return NULL;
    }
    memcpy(client->addr, client_addr, sizeof(struct sockaddr_in));
    client->stream = fdopen(client_sockfd, "r");
    if (client->stream == NULL) {
        perror("fdopen");
        destroy_client(client);
        return NULL;
    }
    write(client->sockfd, "WELCOME\n", 8);
    return client;
}
