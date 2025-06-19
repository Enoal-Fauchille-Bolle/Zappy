/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Debug Option Handler Tests
*/

#include "options_parser_test_utils.h"
#include "options_parser/parser.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// DEBUG OPTION TESTS
// ============================================================================

Test(debug_option, handle_debug_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d"};
    int argc = 2;
    int i = 1;

    handle_debug(options, &i, argc, argv);
    cr_assert_eq(options->debug, true, "Debug flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(debug_option, handle_debug_index_unchanged)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d", "extra_arg"};
    int argc = 3;
    int i = 1;

    handle_debug(options, &i, argc, argv);
    cr_assert_eq(options->debug, true, "Debug flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(debug_option, handle_debug_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d"};
    int argc = 2;
    int i = 1;

    options->debug = true;  // Pre-set debug flag

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_debug(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for already set debug");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(debug_option, handle_debug_state_consistency)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d"};
    int argc = 2;
    int i = 1;

    // Verify initial state
    cr_assert_eq(options->debug, false, "Debug flag should initially be false");
    
    handle_debug(options, &i, argc, argv);
    
    // Verify final state
    cr_assert_eq(options->debug, true, "Debug flag should be set to true");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(debug_option, handle_debug_with_other_options_unset)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d"};
    int argc = 2;
    int i = 1;

    handle_debug(options, &i, argc, argv);
    
    // Verify debug is set and other options remain default
    cr_assert_eq(options->debug, true, "Debug flag should be set to true");
    cr_assert_eq(options->help, false, "Help flag should remain default");
    cr_assert_eq(options->port, 0, "Port should remain default");
    cr_assert_eq(options->width, 0, "Width should remain default");
    cr_assert_eq(options->height, 0, "Height should remain default");
    cr_assert_eq(options->frequency, 0, "Frequency should remain default");
    cr_assert_eq(options->clients_nb, 0, "Clients number should remain default");
    cr_assert_null(options->teams, "Teams should remain NULL");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(debug_option, handle_debug_multiple_args)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d", "-p", "8080", "-x", "10"};
    int argc = 6;
    int i = 1;

    handle_debug(options, &i, argc, argv);
    
    // Verify debug is set and index is correct for next option
    cr_assert_eq(options->debug, true, "Debug flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(debug_option, handle_debug_twice_in_sequence)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-d"};
    int argc = 2;
    int i = 1;

    // First call should succeed
    handle_debug(options, &i, argc, argv);
    cr_assert_eq(options->debug, true, "Debug flag should be set to true after first call");
    cr_assert_eq(options->error, false, "Error flag should remain false after first call");

    // Second call should fail
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_debug(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for second debug call");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}
