/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Main File
*/

#include <stdio.h>
#include <stdlib.h>
#include "options_parser/parser.h"
#include "options_parser/processor.h"
#include "connection/server.h"
#include "connection/socket.h"
#include "constants.h"

int main(int ac, char **av)
{
    server_options_t *options = get_server_options(ac, av);
    server_t *server = NULL;

    if (options == NULL)
        return handle_options_error();
    if (process_options(options) == FAILURE) {
        destroy_server_options(options);
        return EXIT_ERROR_CODE;
    }
    server = create_server(options);
    if (!server)
        return EXIT_ERROR_CODE;
    destroy_server_options(options);
    return EXIT_SUCCESS_CODE;
}
