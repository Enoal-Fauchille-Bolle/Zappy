/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Management
*/

#include "connection/client.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void destroy_client(client_t *client)
{
    if (!client) {
        return;
    }
    if (client->sockfd >= 0) {
        close(client->sockfd);
    }
    if (client->stream) {
        fclose(client->stream);
    }
    if (client->addr) {
        free(client->addr);
    }
    free(client);
}

void process_client_message(struct pollfd *fd, client_t *client)
{
    (void)fd;
    (void)client;
}

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
