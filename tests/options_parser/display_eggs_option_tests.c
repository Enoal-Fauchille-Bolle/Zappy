/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Display Eggs Option Handler Tests
*/

#include "options_parser/parser.h"
#include "options_parser_test_utils.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// DISPLAY EGGS OPTION TESTS
// ============================================================================

Test(display_eggs_option, handle_display_eggs_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-de"};
    int argc = 2;
    int i = 1;

    handle_display_eggs(options, &i, argc, argv);
    cr_assert_eq(options->display_eggs, true, "Display eggs flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(display_eggs_option, handle_display_eggs_index_unchanged)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-de", "extra_arg"};
    int argc = 3;
    int i = 1;

    handle_display_eggs(options, &i, argc, argv);
    cr_assert_eq(options->display_eggs, true, "Display eggs flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(display_eggs_option, handle_display_eggs_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-de"};
    int argc = 2;
    int i = 1;

    options->display_eggs = true; // Pre-set display_eggs flag

    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_display_eggs(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for already set display_eggs");

    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(display_eggs_option, handle_display_eggs_state_consistency)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-de"};
    int argc = 2;
    int i = 1;

    cr_assert_eq(options->display_eggs, false, "Display eggs flag should initially be false");

    handle_display_eggs(options, &i, argc, argv);

    cr_assert_eq(options->display_eggs, true, "Display eggs flag should be set to true");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(display_eggs_option, handle_display_eggs_with_other_options_unset)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-de"};
    int argc = 2;
    int i = 1;

    handle_display_eggs(options, &i, argc, argv);

    cr_assert_eq(options->display_eggs, true, "Display eggs flag should be set to true");
    cr_assert_eq(options->debug, false, "Debug flag should remain default");
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

Test(display_eggs_option, handle_display_eggs_multiple_args)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-de", "-p", "8080", "-x", "10"};
    int argc = 6;
    int i = 1;

    handle_display_eggs(options, &i, argc, argv);

    cr_assert_eq(options->display_eggs, true, "Display eggs flag should be set to true");
    cr_assert_eq(i, 1, "Index should remain the same for flag options");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(display_eggs_option, handle_display_eggs_twice_in_sequence)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-de"};
    int argc = 2;
    int i = 1;

    handle_display_eggs(options, &i, argc, argv);
    cr_assert_eq(options->display_eggs, true,
        "Display eggs flag should be set to true after first call");
    cr_assert_eq(options->error, false,
        "Error flag should remain false after first call");

    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_display_eggs(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for second display_eggs call");

    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}
