/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Option Processor
*/

#include "debug_categories.h"
#include "options_parser/options.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Display the help page for the server.
 *
 * This function prints the usage instructions and available options
 * for the Zappy server. It is called when the user requests help
 * or when mandatory options are missing.
 */
static void print_help_page(void)
{
    printf(
        "USAGE: ./zappy_server -p port -x width -y height -n team1 team2 ... "
        "-c clientsNb -f freq\n");
    printf("OPTIONS:\n");
    printf("  -p port             Port number for the server\n");
    printf("  -x width            Width of the map\n");
    printf("  -y height           Height of the map\n");
    printf("  -n team1 team2 ...  Names of teams\n");
    printf("  -c clientsNb        Number of clients\n");
    printf("  -f freq             Frequency of the server (default: 100)\n");
    printf("  -h --help           Display this help message\n");
    printf("  -d --debug          Enable debug mode\n");
}

/**
 * @brief Display debug information about the server options.
 *
 * This function prints the current server options to the standard output.
 * It is useful for debugging purposes to verify that the options
 * are correctly parsed and set.
 *
 * @param options The server options structure containing the parsed options.
 */
static void option_debug(server_options_t *options)
{
    debug_server(true, "Debugging server options:\n");
    debug_server(true, "Port: %d\n", options->port);
    debug_server(true, "Width: %zu\n", options->width);
    debug_server(true, "Height: %zu\n", options->height);
    for (size_t i = 0; options->teams && options->teams[i]; i++) {
        debug_server(true, "Team %zu: %s\n", i + 1, options->teams[i]);
    }
    debug_server(true, "Clients: %zu\n", options->clients_nb);
    debug_server(true, "Frequency: %u\n", options->frequency);
    debug_server(true, "Help: %d\n", options->help);
    debug_server(true, "Debug: %d\n", options->debug);
    debug_server(true, "Error: %d\n", options->error);
}

/**
 * @brief Handle the error case when option parsing fails.
 *
 * This function is called when the get_server_options() function returns NULL,
 * indicating that option parsing has failed. It prints an error message to
 * stderr and returns the appropriate error code.
 *
 * @return 84 The error code indicating option parsing failure.
 */
int handle_options_error(void)
{
    fprintf(stderr, "Error: Failed to parse options.\n");
    return EXIT_ERROR_CODE;
}

/**
 * @brief Handle help request from command line options.
 *
 * This function checks if the help flag is set in the server options.
 * If help is requested, it displays the help page, frees the options
 * structure, and returns 0 to terminate the program successfully.
 *
 * @param options The server options structure containing the parsed options.
 * @return true if help was requested
 * @return false if help was not requested
 */
static bool handle_help_request(server_options_t *options)
{
    if (options->help) {
        print_help_page();
        return SUCCESS;
    }
    return FAILURE;
}

/**
 * @brief Handle error case in parsed options.
 *
 * This function checks if an error occurred during option parsing.
 * If an error is detected, it displays the help page, frees the options
 * structure, and returns the error code to terminate the program.
 *
 * @param options The server options structure containing the parsed options.
 * @return true if an error was detected (program should exit)
 * @return false if no error was detected (program should continue)
 */
static bool handle_error_case(server_options_t *options)
{
    if (options->error) {
        print_help_page();
        return SUCCESS;
    }
    return FAILURE;
}

/**
 * @brief Process all server options and handle special cases.
 *
 * This function processes the parsed server options by calling various
 * handler functions in sequence. It handles debug mode, help requests,
 * and error cases. If any handler indicates the program should exit,
 * this function returns the appropriate exit code.
 *
 * @param options The server options structure containing the parsed options.
 * @return true if processing was successful
 * @return false if processing failed (e.g., help requested or error detected)
 */
bool process_options(server_options_t *options)
{
    if (options->debug)
        option_debug(options);
    if (handle_help_request(options))
        return FAILURE;
    if (handle_error_case(options))
        return FAILURE;
    return SUCCESS;
}
