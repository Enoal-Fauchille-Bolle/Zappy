/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Structure Header
*/

#ifndef OPTIONS_H_
    #define OPTIONS_H_

    #include <stdbool.h>
    #include <stddef.h>

typedef unsigned int port_t;

typedef struct server_options_s {
    port_t port;
    size_t width;
    size_t height;
    char **teams;
    size_t clients_nb;
    size_t frequency;
    bool help;
    bool debug;
    bool error;
} server_options_t;

#endif /* !OPTIONS_H_ */
