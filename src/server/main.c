/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server Main File
*/

#include <stdio.h>
#include <stdlib.h>
#include "options.h"

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
    printf("Port: %d\n", options->port);
    printf("Width: %zu\n", options->width);
    printf("Height: %zu\n", options->height);
    for (size_t i = 0; options->teams && options->teams[i]; i++) {
        printf("Team %zu: %s\n", i + 1, options->teams[i]);
    }
    printf("Clients: %zu\n", options->clients_nb);
    printf("Frequency: %zu\n", options->frequency);
    printf("Help: %d\n", options->help);
    printf("Debug: %d\n", options->debug);
    printf("Error: %d\n", options->error);
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
static int handle_options_error(void)
{
    fprintf(stderr, "Error: Failed to parse options.\n");
    return 84;
}

/**
 * @brief Handle help request from command line options.
 *
 * This function checks if the help flag is set in the server options.
 * If help is requested, it displays the help page, frees the options
 * structure, and returns 0 to terminate the program successfully.
 *
 * @param options The server options structure containing the parsed options.
 * @return 0 if help was requested (program should exit), -1 to continue
 * execution.
 */
static int handle_help_request(server_options_t *options)
{
    if (options->help) {
        print_help_page();
        free(options);
        return 0;
    }
    return -1;
}

/**
 * @brief Handle error case in parsed options.
 *
 * This function checks if an error occurred during option parsing.
 * If an error is detected, it displays the help page, frees the options
 * structure, and returns the error code to terminate the program.
 *
 * @param options The server options structure containing the parsed options.
 * @return 84 if an error was detected (program should exit), -1 to continue
 * execution.
 */
static int handle_error_case(server_options_t *options)
{
    if (options->error) {
        print_help_page();
        free(options);
        return 84;
    }
    return -1;
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
 * @return 0 for normal execution, positive value for error exit codes.
 */
static int process_options(server_options_t *options)
{
    int result;

    if (options->debug)
        option_debug(options);
    result = handle_help_request(options);
    if (result != -1)
        return result;
    result = handle_error_case(options);
    if (result != -1)
        return result;
    return 0;
}

int main(int ac, char **av)
{
    server_options_t *options = get_server_options(ac, av);
    int result = 0;

    if (options == NULL)
        return handle_options_error();
    result = process_options(options);
    if (result != 0) {
        return result;
    }
    destroy_server_options(options);
    return 0;
}
