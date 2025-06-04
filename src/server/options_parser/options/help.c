/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Help Option
*/

#include "options_parser/parser.h"
#include <stdbool.h>

/**
 * @brief Handle the help option parsing from command line arguments.
 *
 * This function processes the "-h" or "--help" options to enable
 * help mode. When help is requested, the program will display
 * usage information and exit without starting the server.
 *
 * @param options Pointer to the server options structure to modify
 * @param i Pointer to the current argument index (unused)
 * @param ac Total number of command line arguments (unused)
 * @param av Array of command line argument strings (unused)
 *
 * @note This is a flag option that doesn't consume additional arguments
 */
void handle_help(server_options_t *options, int *i, int ac, char **av)
{
    (void)i;
    (void)ac;
    (void)av;
    options->help = true;
}
