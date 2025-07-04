/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Height Option
*/

#include "constants.h"
#include "options_parser/options.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Handle the height option parsing from command line arguments.
 *
 * This function processes the "-y" option to set the map height.
 * It validates that the height value is within the valid range
 * (1-MAX_MAP_SIZE) and sets an error flag if validation fails.
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
    if (options->height != 0) {
        fputs("Error: Height option already set\n", stderr);
        options->error = true;
        *i += 1;
        return;
    }
    if (*i + 1 < ac) {
        *i += 1;
        options->height = atoi(av[*i]);
        if (options->height <= 0 || options->height > MAX_MAP_SIZE) {
            fprintf(stderr,
                "Error: Height must be a number between 1 and %d\n",
                MAX_MAP_SIZE);
            options->error = true;
        }
    } else {
        fputs("Error: Missing height value\n", stderr);
        options->error = true;
    }
}
