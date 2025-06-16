/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Width Option
*/

#include "options_parser/options.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Handle the width option parsing from command line arguments.
 *
 * This function processes the "-x" option to set the map width.
 * It validates that the width value is within the valid range (1-1000)
 * and sets an error flag if validation fails.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (will be incremented)
 * @param ac Total number of command line arguments
 * @param av Array of command line argument strings
 *
 * @note Sets options->error to true if width value is missing or invalid
 */
void handle_width(server_options_t *options, int *i, int ac, char **av)
{
    if (options->width != 0) {
        fputs("Error: Width option already set\n", stderr);
        options->error = true;
        *i += 1;
        return;
    }
    if (*i + 1 < ac) {
        *i += 1;
        options->width = atoi(av[*i]);
        if (options->width <= 0 || options->width > 1000) {
            fputs(
                "Error: Width must be a number between 1 and 1000\n", stderr);
            options->error = true;
        }
    } else {
        fputs("Error: Missing width value\n", stderr);
        options->error = true;
    }
}
