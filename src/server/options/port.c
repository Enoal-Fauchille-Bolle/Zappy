/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Port Option
*/

#include "options.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Handle the port option parsing from command line arguments.
 *
 * This function processes the "-p" option to set the server port number.
 * It validates that the port value is within the valid range (1-65535)
 * and sets an error flag if validation fails.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (will be incremented)
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 *
 * @note Sets options->error to true if port value is missing or invalid
 */
void handle_port(server_options_t *options, int *i, int ac, char **av)
{
    if (*i + 1 < ac) {
        *i += 1;
        options->port = atoi(av[*i]);
        if (options->port <= 0 || options->port > 65535) {
            dprintf(fileno(stderr),
                    "Error: Port must be a number between 1 and 65535\n");
            options->error = true;
        }
    } else {
        dprintf(fileno(stderr), "Error: Missing port value\n");
        options->error = true;
    }
}
