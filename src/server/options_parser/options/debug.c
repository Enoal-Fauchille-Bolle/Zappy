/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Debug Option
*/

#include "options_parser/options.h"
#include <stdbool.h>

/**
 * @brief Handle the debug option parsing from command line arguments.
 *
 * This function processes the "-d" or "--debug" options to enable
 * debug mode. When debug mode is enabled, the server will output
 * additional diagnostic information during execution.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (unused)
 * @param ac Total number of command line arguments (unused)
 * @param av Array of command line argument strings (unused)
 *
 * @note This is a flag option that doesn't consume additional arguments
 */
void handle_debug(server_options_t *options, int *i, int ac, char **av)
{
    (void)i;
    (void)ac;
    (void)av;
    options->debug = true;
}
