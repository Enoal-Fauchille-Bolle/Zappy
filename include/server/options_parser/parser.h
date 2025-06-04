/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Options
*/

#ifndef OPTIONS_PARSER_H_
    #define OPTIONS_PARSER_H_

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

typedef void (*option_handler_t)(server_options_t *opts, int *i, int ac,
    char **av);

typedef struct option_map_s {
    const char *option;
    option_handler_t handler;
} option_map_t;

// Option handlers
void handle_port(server_options_t *opts, int *i, int ac, char **av);
void handle_width(server_options_t *opts, int *i, int ac, char **av);
void handle_height(server_options_t *opts, int *i, int ac, char **av);
void handle_teams(server_options_t *options, int *i, int ac, char **av);
void handle_clients(server_options_t *options, int *i, int ac, char **av);
void handle_frequency(server_options_t *options, int *i, int ac, char **av);
void handle_help(server_options_t *options, int *i, int ac, char **av);
void handle_debug(server_options_t *options, int *i, int ac, char **av);

server_options_t *get_server_options(int ac, char **av);
void destroy_server_options(server_options_t *options);

#endif /* !OPTIONS_PARSER_H_ */
