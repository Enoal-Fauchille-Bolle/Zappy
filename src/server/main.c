/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Main File
*/

#include "constants.h"
#include "options_parser/parser.h"
#include "options_parser/processor.h"
#include <stdlib.h>
#include <time.h>

int main(int ac, char **av)
{
    server_options_t *options = get_server_options(ac, av);

    if (options == NULL)
        return handle_options_error();
    if (process_options(options) == FAILURE) {
        destroy_server_options(options);
        return EXIT_ERROR_CODE;
    }
    srand(time(NULL));
    destroy_server_options(options);
    return EXIT_SUCCESS_CODE;
}
