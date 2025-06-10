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

typedef struct server_s server_t;

typedef struct client_s {
    int sockfd;
    server_t *server;
    struct sockaddr_in addr;
    FILE *stream;
} client_t;

#endif /* !CLIENT_H_ */
