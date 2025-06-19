/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Height Option Handler Tests
*/

#include "options_parser_test_utils.h"
#include "options_parser/parser.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// HEIGHT OPTION TESTS
// ============================================================================

Test(height_option, handle_height_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", "20"};
    int argc = 3;
    int i = 1;

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->height, 20, "Height should be set to 20");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(height_option, handle_height_valid_large)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", "100"};
    int argc = 3;
    int i = 1;

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->height, 100, "Height should be set to 100");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(height_option, handle_height_minimum_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", "1"};
    int argc = 3;
    int i = 1;

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->height, 1, "Height should be set to 1");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(height_option, handle_height_invalid_zero)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", "0"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for zero height");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(height_option, handle_height_invalid_negative)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", "-10"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for negative height");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(height_option, handle_height_invalid_non_numeric)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", "abc"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for non-numeric height");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(height_option, handle_height_missing_value)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y"};
    int argc = 2;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for missing height value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(height_option, handle_height_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", "20"};
    int argc = 3;
    int i = 1;

    options->height = 15;  // Pre-set height

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for already set height");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(height_option, handle_height_empty_string)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-y", ""};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_height(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for empty height value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}
