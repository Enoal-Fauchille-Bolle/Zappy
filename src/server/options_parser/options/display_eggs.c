/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Display eggs Option
*/

#include "options_parser/options.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Handle the display eggs option parsing from command line arguments.
 *
 * This function processes the "-de" or "--display-eggs" options to enable
 * displaying eggs in the game. It sets an error flag if the option is already
 * set or if it is used incorrectly.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (unused)
 * @param ac Total number of command line arguments (unused)
 * @param av Array of command line argument strings (unused)
 *
 * @note This is a flag option that doesn't consume additional arguments
 */
void handle_display_eggs(server_options_t *options, int *i, int ac, char **av)
{
    (void)i;
    (void)ac;
    (void)av;
    if (options->display_eggs) {
        fputs("Error: Display eggs option already set\n", stderr);
        options->error = true;
        return;
    }
    options->display_eggs = true;
}
