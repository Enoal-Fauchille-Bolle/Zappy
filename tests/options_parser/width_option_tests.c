/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Width Option Handler Tests
*/

#include "options_parser/parser.h"
#include "options_parser_test_utils.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// WIDTH OPTION TESTS
// ============================================================================

Test(width_option, handle_width_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", "15"};
    int argc = 3;
    int i = 1;

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->width, 15, "Width should be set to 15");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(width_option, handle_width_valid_large)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", "100"};
    int argc = 3;
    int i = 1;

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->width, 100, "Width should be set to 100");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(width_option, handle_width_minimum_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", "1"};
    int argc = 3;
    int i = 1;

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->width, 1, "Width should be set to 1");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(width_option, handle_width_invalid_zero)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", "0"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for zero width");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(width_option, handle_width_invalid_negative)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", "-5"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for negative width");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(width_option, handle_width_invalid_non_numeric)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", "abc"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for non-numeric width");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(width_option, handle_width_missing_value)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x"};
    int argc = 2;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for missing width value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(width_option, handle_width_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", "15"};
    int argc = 3;
    int i = 1;

    options->width = 10;      // Pre-set width

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for already set width");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(width_option, handle_width_empty_string)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-x", ""};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_width(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for empty width value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}
