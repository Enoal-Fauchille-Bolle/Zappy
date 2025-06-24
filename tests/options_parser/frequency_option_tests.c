/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Frequency Option Handler Tests
*/

#include "options_parser/parser.h"
#include "options_parser_test_utils.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// FREQUENCY OPTION TESTS
// ============================================================================

Test(frequency_option, handle_frequency_valid_min)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "1"};
    int argc = 3;
    int i = 1;

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->frequency, 1, "Frequency should be set to 1");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_valid_mid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "150"};
    int argc = 3;
    int i = 1;

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->frequency, 150, "Frequency should be set to 150");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_valid_max)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "1000"};
    int argc = 3;
    int i = 1;

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->frequency, 1000, "Frequency should be set to 1000");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_invalid_zero)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "0"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for frequency zero");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_invalid_negative)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "-5"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for negative frequency");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_invalid_too_high)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "10001"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for frequency too high");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_invalid_non_numeric)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "abc"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for non-numeric frequency");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_missing_value)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f"};
    int argc = 2;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for missing frequency value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", "100"};
    int argc = 3;
    int i = 1;

    options->frequency = 50;      // Pre-set frequency

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for already set frequency");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(frequency_option, handle_frequency_empty_string)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-f", ""};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_frequency(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for empty frequency value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}
