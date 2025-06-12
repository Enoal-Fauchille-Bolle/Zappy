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
    #include "options_parser/options.h"

typedef void (*option_handler_t)(server_options_t *opts, int *i, int ac,
    char **av);

typedef struct option_map_s {
    const char *option;
    option_handler_t handler;
} option_map_t;

server_options_t *init_options(void);
void parse_options(server_options_t *options, int ac, char **av);
bool are_mandatory_options_present(server_options_t *options);

// Option handlers
void handle_port(server_options_t *opts, int *i, int ac, char **av);
void handle_width(server_options_t *opts, int *i, int ac, char **av);
void handle_height(server_options_t *opts, int *i, int ac, char **av);
void handle_teams(server_options_t *options, int *i, int ac, char **av);
void handle_clients(server_options_t *options, int *i, int ac, char **av);
void handle_frequency(server_options_t *options, int *i, int ac, char **av);
void handle_help(server_options_t *options, int *i, int ac, char **av);
void handle_debug(server_options_t *options, int *i, int ac, char **av);

#endif /* !OPTIONS_PARSER_H_ */
