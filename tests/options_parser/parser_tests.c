/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Core Parser Tests
*/

#include "options_parser/parser.h"
#include "options_parser/processor.h"
#include "options_parser_test_utils.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// INITIALIZATION TESTS
// ============================================================================

Test(options_parser, init_options_success)
{
    server_options_t *options = init_options();

    cr_assert_not_null(options, "init_options should not return NULL");
    cr_assert_eq(options->port, 0, "Port should be initialized to 0");
    cr_assert_eq(options->width, 0, "Width should be initialized to 0");
    cr_assert_eq(options->height, 0, "Height should be initialized to 0");
    cr_assert_null(options->teams, "Teams should be initialized to NULL");
    cr_assert_eq(
        options->clients_nb, 0, "Clients number should be initialized to 0");
    cr_assert_eq(
        options->frequency, 0, "Frequency should be initialized to 0");
    cr_assert_eq(options->help, false, "Help should be initialized to false");
    cr_assert_eq(
        options->debug, false, "Debug should be initialized to false");
    cr_assert_eq(
        options->error, false, "Error should be initialized to false");

    cleanup_server_options(options);
}

// ============================================================================
// VALIDATION TESTS
// ============================================================================

Test(options_parser, are_mandatory_options_present_valid)
{
    server_options_t *options = init_options();
    const char *team_names[] = {"team1", "team2"};

    options->port = 8080;
    options->width = 10;
    options->height = 10;
    options->teams = create_teams_array(team_names, 2);

    cr_assert_eq(are_mandatory_options_present(options), true,
        "Should return true when all mandatory options are present");

    cleanup_server_options(options);
}

Test(options_parser, are_mandatory_options_present_help_mode)
{
    server_options_t *options = init_options();

    options->help = true;

    cr_assert_eq(are_mandatory_options_present(options), true,
        "Should return true when help flag is set");

    cleanup_server_options(options);
}

Test(options_parser, are_mandatory_options_present_missing_port)
{
    server_options_t *options = init_options();
    const char *team_names[] = {"team1", "team2"};

    options->width = 10;
    options->height = 10;
    options->teams = create_teams_array(team_names, 2);

    cr_assert_eq(are_mandatory_options_present(options), false,
        "Should return false when port is missing");

    cleanup_server_options(options);
}

Test(options_parser, are_mandatory_options_present_missing_width)
{
    server_options_t *options = init_options();
    const char *team_names[] = {"team1", "team2"};

    options->port = 8080;
    options->height = 10;
    options->teams = create_teams_array(team_names, 2);

    cr_assert_eq(are_mandatory_options_present(options), false,
        "Should return false when width is missing");

    cleanup_server_options(options);
}

Test(options_parser, are_mandatory_options_present_missing_teams)
{
    server_options_t *options = init_options();

    options->port = 8080;
    options->width = 10;
    options->height = 10;

    cr_assert_eq(are_mandatory_options_present(options), false,
        "Should return false when teams are missing");

    cleanup_server_options(options);
}

// ============================================================================
// PARSING TESTS
// ============================================================================

Test(options_parser, parse_options_valid_arguments)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "8080", "-x", "10", "-y", "15", "-c",
        "5", "-f", "100", "-n", "team1", "team2"};
    int argc = 13;

    parse_options(options, argc, argv);
    cr_assert_eq(options->error, false, "Error flag should be false");

    cleanup_server_options(options);
}

Test(options_parser, parse_options_help_short)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-h", NULL};
    int argc = 2;

    parse_options(options, argc, argv);
    cr_assert_eq(options->help, true, "Help flag should be set to true");

    cleanup_server_options(options);
}

Test(options_parser, parse_options_help_long)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "--help", NULL};
    int argc = 2;

    parse_options(options, argc, argv);
    cr_assert_eq(options->help, true, "Help flag should be set to true");

    cleanup_server_options(options);
}

Test(options_parser, parse_options_debug_short)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d", NULL};
    int argc = 2;

    parse_options(options, argc, argv);
    cr_assert_eq(options->debug, true, "Debug flag should be set to true");

    cleanup_server_options(options);
}

Test(options_parser, parse_options_debug_long)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "--debug", NULL};
    int argc = 2;

    parse_options(options, argc, argv);
    cr_assert_eq(options->debug, true, "Debug flag should be set to true");

    cleanup_server_options(options);
}

Test(options_parser, parse_options_empty_args)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", NULL};
    int argc = 1;

    parse_options(options, argc, argv);
    cr_assert_eq(options->error, false, "Error flag should be false");

    cleanup_server_options(options);
}

Test(options_parser, parse_options_mixed_order)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "50", "-p", "9000", "-y", "20", "-x",
        "15", "-c", "3", "-n", "alpha", "beta", "gamma", NULL};
    int argc = 13;

    parse_options(options, argc, argv);
    cr_assert_eq(options->port, 9000, "Port should be set correctly");
    cr_assert_eq(options->width, 15, "Width should be set correctly");
    cr_assert_eq(options->height, 20, "Height should be set correctly");
    cr_assert_eq(options->frequency, 50, "Frequency should be set correctly");
    cr_assert_eq(
        options->clients_nb, 3, "Clients number should be set correctly");

    cleanup_server_options(options);
}

// ============================================================================
// CLEANUP TESTS
// ============================================================================

Test(options_parser, destroy_server_options_null)
{
    // This should not crash
    destroy_server_options(NULL);
    cr_assert(true, "Should handle NULL pointer gracefully");
}

Test(options_parser, destroy_server_options_valid)
{
    server_options_t *options = init_options();
    const char *team_names[] = {"team1", "team2"};

    options->teams = create_teams_array(team_names, 2);

    // This should not crash and should free all allocated memory
    destroy_server_options(options);
    cr_assert(true, "Should cleanup allocated memory successfully");
}
