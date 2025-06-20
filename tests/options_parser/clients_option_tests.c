/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Clients Option Handler Tests
*/

#include "options_parser_test_utils.h"
#include "options_parser/parser.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// CLIENTS OPTION TESTS
// ============================================================================

Test(clients_option, handle_clients_valid_min)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", "1"};
    int argc = 3;
    int i = 1;

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->clients_nb, 1, "Clients number should be set to 1");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_valid)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", "5"};
    int argc = 3;
    int i = 1;

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->clients_nb, 5, "Clients number should be set to 5");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_valid_large)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", "100"};
    int argc = 3;
    int i = 1;

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->clients_nb, 100, "Clients number should be set to 100");
    cr_assert_eq(i, 2, "Index should be incremented to 2");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_invalid_zero)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", "0"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for zero clients");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_invalid_negative)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", "-5"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for negative clients");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_invalid_non_numeric)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", "abc"};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for non-numeric clients");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_missing_value)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c"};
    int argc = 2;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for missing clients value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", "5"};
    int argc = 3;
    int i = 1;

    options->clients_nb = 3;  // Pre-set clients number

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for already set clients");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(clients_option, handle_clients_empty_string)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-c", ""};
    int argc = 3;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_clients(options, &i, argc, argv);
    cr_assert_eq(options->error, true, "Error flag should be set to true for empty clients value");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}
