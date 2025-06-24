/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Processor Tests
*/

#include "options_parser/parser.h"
#include "options_parser/processor.h"
#include "options_parser_test_utils.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// PROCESSING TESTS
// ============================================================================

Test(options_parser, process_options_success)
{
    server_options_t *options = init_options();
    const char *team_names[] = {"team1", "team2"};

    options->port = 8080;
    options->width = 10;
    options->height = 10;
    options->teams = create_teams_array(team_names, 2);
    options->clients_nb = 5;
    options->frequency = 100;

    cr_assert_eq(process_options(options), true,
        "Should return true for valid options");

    cleanup_server_options(options);
}

Test(options_parser, process_options_help_request)
{
    server_options_t *options = init_options();

    options->help = true;

    // Redirect stdout to avoid output during tests
    int stdout_backup = dup(STDOUT_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDOUT_FILENO);

    cr_assert_eq(process_options(options), false,
        "Should return false for help request");

    // Restore stdout
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(options_parser, process_options_error_case)
{
    server_options_t *options = init_options();

    options->error = true;

    cr_assert_eq(process_options(options), false,
        "Should return false when error flag is set");

    cleanup_server_options(options);
}

Test(options_parser, process_options_valid_no_flags)
{
    server_options_t *options = init_options();
    const char *team_names[] = {"team1", "team2"};

    options->port = 8080;
    options->width = 10;
    options->height = 10;
    options->teams = create_teams_array(team_names, 2);
    options->clients_nb = 5;
    options->frequency = 100;
    // No debug, help, or error flags set

    cr_assert_eq(process_options(options), true,
        "Should return true when no special flags are set");

    cleanup_server_options(options);
}

Test(options_parser, process_options_debug_output)
{
    server_options_t *options = init_options();
    const char *team_names[] = {"team1", "team2"};

    options->port = 8080;
    options->width = 10;
    options->height = 10;
    options->teams = create_teams_array(team_names, 2);
    options->clients_nb = 5;
    options->frequency = 100;
    options->debug = true;

    // Test that debug output is generated when debug flag is set
    // Since option_debug is static, we test it indirectly through
    // process_options
    cr_assert_eq(process_options(options), true,
        "Should return true when debug flag is set");

    cleanup_server_options(options);
}

Test(options_parser, process_options_debug_with_null_teams)
{
    server_options_t *options = init_options();

    options->port = 8080;
    options->width = 10;
    options->height = 10;
    options->teams = NULL;      // Test NULL teams handling in debug output
    options->clients_nb = 5;
    options->frequency = 100;
    options->debug = true;

    // Should handle NULL teams gracefully in debug output
    cr_assert_eq(process_options(options), true,
        "Should return true - process_options doesn't validate options");

    cleanup_server_options(options);
}

Test(options_parser, process_options_debug_with_empty_teams)
{
    server_options_t *options = init_options();

    options->port = 8080;
    options->width = 10;
    options->height = 10;
    options->teams = malloc(sizeof(char *) * 1);
    options->teams[0] = NULL;      // Empty teams array
    options->clients_nb = 5;
    options->frequency = 100;
    options->debug = true;

    // Should handle empty teams gracefully in debug output
    cr_assert_eq(process_options(options), true,
        "Should return true - process_options doesn't validate options");

    cleanup_server_options(options);
}

// ============================================================================
// ERROR HANDLING TESTS
// ============================================================================

Test(options_parser, handle_options_error_return_code)
{
    cr_assert_eq(handle_options_error(), 84, "Should return error code 84");
}
