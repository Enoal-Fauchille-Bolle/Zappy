/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Main File
*/

#include "connection/server.h"
#include "constants.h"
#include "options_parser/processor.h"
#include <stdlib.h>
#include <time.h>

/**
 * @brief Gets the appropriate return value based on server options error state
 *
 * @param options Pointer to server options structure containing error state
 * @return EXIT_ERROR_CODE if an error occurred, EXIT_SUCCESS_CODE otherwise
 *
 * @note This function destroys the server options structure before returning
 */
static int get_return_value(server_options_t *options)
{
    bool error = options->error;

    destroy_server_options(options);
    if (error)
        return EXIT_ERROR_CODE;
    return EXIT_SUCCESS_CODE;
}

int main(int ac, char **av)
{
    server_options_t *options = get_server_options(ac, av);
    server_t *server = NULL;

    if (options == NULL)
        return handle_options_error();
    if (process_options(options) == FAILURE) {
        return get_return_value(options);
    }
    server = create_server(options);
    if (!server)
        return EXIT_ERROR_CODE;
    srand(time(NULL));
    run_server(server, options->port);
    return EXIT_SUCCESS_CODE;
}
