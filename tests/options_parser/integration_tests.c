/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Integration Tests
*/

#include "options_parser_test_utils.h"
#include "options_parser/options.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// INTEGRATION TESTS
// ============================================================================

Test(options_parser, get_server_options_valid)
{
    char *argv[] = {
        "zappy_server",
        "-p", "8080",
        "-x", "10",
        "-y", "15",
        "-c", "5",
        "-f", "100",
        "-n", "team1", "team2"
    };
    int argc = 14;

    server_options_t *options = get_server_options(argc, argv);

    cr_assert_not_null(options, "get_server_options should not return NULL for valid arguments");
    cr_assert_eq(options->error, false, "Error should be false for valid arguments");
    cr_assert_eq(options->port, 8080, "Port should be set correctly");
    cr_assert_eq(options->width, 10, "Width should be set correctly");
    cr_assert_eq(options->height, 15, "Height should be set correctly");
    cr_assert_eq(options->clients_nb, 5, "Clients number should be set correctly");
    cr_assert_eq(options->frequency, 100, "Frequency should be set correctly");
    cr_assert_not_null(options->teams, "Teams should be set");
    cr_assert_str_eq(options->teams[0], "team1", "First team should be correct");
    cr_assert_str_eq(options->teams[1], "team2", "Second team should be correct");

    cleanup_server_options(options);
}

Test(options_parser, get_server_options_default_frequency)
{
    char *argv[] = {
        "zappy_server",
        "-p", "8080",
        "-x", "10",
        "-y", "15",
        "-n", "team1", "team2",
        "-c", "5",
        NULL
    };
    int argc = 9;

    // Redirect stdout to avoid debug output during tests
    int stdout_backup = dup(STDOUT_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDOUT_FILENO);

    server_options_t *options = get_server_options(argc, argv);

    // Restore stdout
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
    close(dev_null);

    cr_assert_not_null(options, "get_server_options should not return NULL");
    cr_assert_eq(options->frequency, 100, "Default frequency should be 100");

    cleanup_server_options(options);
}

Test(options_parser, get_server_options_missing_mandatory)
{
    char *argv[] = {
        "zappy_server",
        "-p", "8080"
        // Missing width, height, and teams
    };
    int argc = 3;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    server_options_t *options = get_server_options(argc, argv);

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cr_assert_not_null(options, "get_server_options should return pointer even on error");
    cr_assert_eq(options->error, true, "Error flag should be true for missing mandatory options");
}

Test(options_parser, get_server_options_help_request)
{
    char *argv[] = {"zappy_server", "-h", NULL};
    int argc = 2;

    // Redirect stdout to avoid output during tests
    int stdout_backup = dup(STDOUT_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDOUT_FILENO);

    server_options_t *options = get_server_options(argc, argv);

    // Restore stdout
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdout_backup);
    close(dev_null);

    cr_assert_not_null(options, "get_server_options should not return NULL for help request");
    cr_assert_eq(options->help, true, "Help flag should be set");

    cleanup_server_options(options);
}

Test(options_parser, get_server_options_debug_request)
{
    char *argv[] = {
        "zappy_server",
        "-p", "8080",
        "-x", "10",
        "-y", "15",
        "-n", "team1",
        "-c", "5",
        "-f", "100",
        "-d"
    };
    int argc = 14;

    server_options_t *options = get_server_options(argc, argv);

    cr_assert_not_null(options, "get_server_options should not return NULL for debug request");
    cr_assert_eq(options->debug, true, "Debug flag should be set");
    cr_assert_eq(options->error, false, "Error flag should be false");

    cleanup_server_options(options);
}

Test(options_parser, get_server_options_invalid_arguments)
{
    char *argv[] = {
        "zappy_server",
        "-p", "0",  // Invalid port
        "-x", "10",
        "-y", "15",
        "-n", "team1"
    };
    int argc = 9;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    server_options_t *options = get_server_options(argc, argv);

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cr_assert_not_null(options, "get_server_options should return pointer even on error");
    cr_assert_eq(options->error, true, "Error flag should be true for invalid arguments");

    cleanup_server_options(options);
}

Test(options_parser, get_server_options_complex_scenario)
{
    char *argv[] = {
        "zappy_server",
        "--debug",
        "-p", "9999",
        "-x", "25",
        "-y", "30",
        "-n", "alpha", "beta", "gamma", "delta",
        "-c", "10",
        "-f", "500"
    };
    int argc = 17;

    server_options_t *options = get_server_options(argc, argv);

    cr_assert_not_null(options, "get_server_options should handle complex scenario");
    cr_assert_eq(options->error, false, "Error flag should be false");
    cr_assert_eq(options->debug, true, "Debug flag should be set");
    cr_assert_eq(options->port, 9999, "Port should be set correctly");
    cr_assert_eq(options->width, 25, "Width should be set correctly");
    cr_assert_eq(options->height, 30, "Height should be set correctly");
    cr_assert_eq(options->clients_nb, 10, "Clients number should be set correctly");
    cr_assert_eq(options->frequency, 500, "Frequency should be set correctly");
    cr_assert_not_null(options->teams, "Teams should be set");

    // Check teams
    for (int i = 0; i < 4; i++) {
        cr_assert_not_null(options->teams[i], "Team %d should exist", i);
    }
    cr_assert_str_eq(options->teams[0], "alpha", "First team should be alpha");
    cr_assert_str_eq(options->teams[1], "beta", "Second team should be beta");
    cr_assert_str_eq(options->teams[2], "gamma", "Third team should be gamma");
    cr_assert_str_eq(options->teams[3], "delta", "Fourth team should be delta");
    cr_assert_null(options->teams[4], "Teams array should be null-terminated");

    cleanup_server_options(options);
}
