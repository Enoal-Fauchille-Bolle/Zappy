/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Port Option Handler Tests
*/

#include "options_parser_test_utils.h"
#include "options_parser/parser.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// PORT OPTION TESTS
// ============================================================================

Test(port_option, handle_port_valid_min_range)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "1024"};
    int argc = 3;
    int i = 1;

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->port, 1024, "Port should be set to 1024");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(port_option, handle_port_valid_mid_range)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "8080"};
    int argc = 3;
    int i = 1;

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->port, 8080, "Port should be set to 8080");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(port_option, handle_port_valid_max_range)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "65535"};
    int argc = 3;
    int i = 1;

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->port, 65535, "Port should be set to 65535");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(port_option, handle_port_invalid_range_low)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "1023"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set for port < 1024");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(port_option, handle_port_invalid_range_high)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "65536"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set for port > 65535");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(port_option, handle_port_invalid_zero)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "0"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set for port 0");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(port_option, handle_port_invalid_negative)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "-1"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set for negative port");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(port_option, handle_port_invalid_non_numeric)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "abc"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set for non-numeric port");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(port_option, handle_port_missing_value)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p"};
    int argc = 2;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set for missing port value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(port_option, handle_port_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "9090"};
    int argc = 3;
    int i = 1;

    options->port = 8080; // Pre-set port

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set for already set port");
    cr_assert_eq(options->port, 8080, "Port should remain unchanged");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(port_option, handle_port_boundary_values)
{
    // Test edge case just above minimum
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-p", "1025"};
    int argc = 3;
    int i = 1;

    handle_port(options, &i, argc, argv);
    cr_assert_eq(options->port, 1025, "Port 1025 should be valid");
    cr_assert_eq(options->error, false, "Error flag should be false for port 1025");

    cleanup_server_options(options);

    // Test edge case just below maximum
    options = init_options();
    char *argv2[] = {"zappy_server", "-p", "65534"};
    i = 1;

    handle_port(options, &i, argc, argv2);
    cr_assert_eq(options->port, 65534, "Port 65534 should be valid");
    cr_assert_eq(options->error, false, "Error flag should be false for port 65534");

    cleanup_server_options(options);
}
