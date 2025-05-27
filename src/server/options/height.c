/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Height Option
*/

#include "options.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief Handle the height option parsing from command line arguments.
 *
 * This function processes the "-y" option to set the map height.
 * It validates that the height value is within the valid range (1-1000)
 * and sets an error flag if validation fails.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (will be incremented)
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 *
 * @note Sets options->error to true if height value is missing or invalid
 */
void handle_height(server_options_t *options, int *i, int ac, char **av)
{
    if (*i + 1 < ac) {
        *i += 1;
        options->height = atoi(av[*i]);
        if (options->height <= 0 || options->height > 1000) {
            dprintf(fileno(stderr),
                    "Error: Height must be a number between 1 and 1000\n");
            options->error = true;
        }
    } else {
        dprintf(fileno(stderr), "Error: Missing height value\n");
        options->error = true;
    }
}
