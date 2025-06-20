/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Help Option Handler Tests
*/

#include "options_parser/parser.h"
#include "options_parser_test_utils.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// HELP OPTION TESTS
// ============================================================================

Test(help_option, handle_help_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-h"};
    int argc = 2;
    int i = 1;

    handle_help(options, &i, argc, argv);
    cr_assert_eq(options->help, true, "Help flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(help_option, handle_help_index_unchanged)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-h", "extra_arg"};
    int argc = 3;
    int i = 1;

    handle_help(options, &i, argc, argv);
    cr_assert_eq(options->help, true, "Help flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(help_option, handle_help_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-h"};
    int argc = 2;
    int i = 1;

    options->help = true;      // Pre-set help flag

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_help(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for already set help");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(help_option, handle_help_state_consistency)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-h"};
    int argc = 2;
    int i = 1;

    // Verify initial state
    cr_assert_eq(options->help, false, "Help flag should initially be false");

    handle_help(options, &i, argc, argv);

    // Verify final state
    cr_assert_eq(options->help, true, "Help flag should be set to true");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(help_option, handle_help_with_other_options_unset)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-h"};
    int argc = 2;
    int i = 1;

    handle_help(options, &i, argc, argv);

    // Verify help is set and other options remain default
    cr_assert_eq(options->help, true, "Help flag should be set to true");
    cr_assert_eq(options->port, 0, "Port should remain default");
    cr_assert_eq(options->width, 0, "Width should remain default");
    cr_assert_eq(options->height, 0, "Height should remain default");
    cr_assert_eq(options->frequency, 0, "Frequency should remain default");
    cr_assert_eq(
        options->clients_nb, 0, "Clients number should remain default");
    cr_assert_null(options->teams, "Teams should remain NULL");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}
