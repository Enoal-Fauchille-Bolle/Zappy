/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Client Structure Header
*/

#ifndef CLIENT_H_
    #define CLIENT_H_

    #include <netinet/in.h>
    #include <stdio.h>
    #include <poll.h>
    #include <errno.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <stdbool.h>

typedef struct server_s server_t;

typedef struct client_s {
    int sockfd;
    server_t *server;
    struct sockaddr_in *addr;
    FILE *stream;
} client_t;

client_t *init_client(
    server_t *server, int client_sockfd, struct sockaddr_in *client_addr);
void process_client_message(client_t *client, struct pollfd *fds,
    int client_index);
void destroy_client(client_t *client);

#endif /* !CLIENT_H_ */
