/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Clients Option
*/

#include "options.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Handle the clients option parsing from command line arguments.
 *
 * This function processes the "-c" option to set the maximum number
 * of clients that can connect to the server. It validates that the
 * number is positive and sets an error flag if validation fails.
 *
 * @param opts Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (will be incremented)
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 *
 * @note Sets opts->error to true if clients number is missing or non-positive
 */
void handle_clients(server_options_t *opts, int *i, int ac, char **av)
{
    if (*i + 1 < ac) {
        *i += 1;
        opts->clients_nb = atoi(av[*i]);
        if (opts->clients_nb <= 0) {
            dprintf(fileno(stderr),
                    "Error: Clients number must be positive\n");
            opts->error = true;
        }
    } else {
        dprintf(fileno(stderr), "Error: Missing clients number\n");
        opts->error = true;
    }
}
