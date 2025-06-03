/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Port Option
*/

#include "options.h"
#include <stdio.h>
#include <stdlib.h>

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
        if (options->port < 1024 || options->port > 65535) {
            fputs("Error: Port must be a number between 1024 and 65535\n",
                stderr);
            options->error = true;
        }
    } else {
        fputs("Error: Missing port value\n", stderr);
        options->error = true;
    }
}
