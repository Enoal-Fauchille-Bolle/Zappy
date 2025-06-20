/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Options Parser - Teams Option Handler Tests
*/

#include "options_parser/parser.h"
#include "options_parser_test_utils.h"
#include <criterion/criterion.h>
#include <fcntl.h>
#include <unistd.h>

// ============================================================================
// TEAMS OPTION TESTS
// ============================================================================

Test(teams_option, handle_teams_single_team)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-n", "team1", "-p", "8080"};
    int argc = 5;
    int i = 1;

    handle_teams(options, &i, argc, argv);
    cr_assert_not_null(options->teams, "Teams array should not be NULL");
    cr_assert_str_eq(
        options->teams[0], "team1", "First team should be 'team1'");
    cr_assert_null(options->teams[1], "Teams array should be null-terminated");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_multiple_teams)
{
    server_options_t *options = init_options();
    char *argv[] = {
        "zappy_server", "-n", "team1", "team2", "team3", "-p", "8080"};
    int argc = 7;
    int i = 1;

    handle_teams(options, &i, argc, argv);
    cr_assert_not_null(options->teams, "Teams array should not be NULL");
    cr_assert_str_eq(
        options->teams[0], "team1", "First team should be 'team1'");
    cr_assert_str_eq(
        options->teams[1], "team2", "Second team should be 'team2'");
    cr_assert_str_eq(
        options->teams[2], "team3", "Third team should be 'team3'");
    cr_assert_null(options->teams[3], "Teams array should be null-terminated");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_two_teams)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-n", "teamA", "teamB", "-p", "8080"};
    int argc = 6;
    int i = 1;

    handle_teams(options, &i, argc, argv);
    cr_assert_not_null(options->teams, "Teams array should not be NULL");
    cr_assert_str_eq(
        options->teams[0], "teamA", "First team should be 'teamA'");
    cr_assert_str_eq(
        options->teams[1], "teamB", "Second team should be 'teamB'");
    cr_assert_null(options->teams[2], "Teams array should be null-terminated");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_with_special_chars)
{
    server_options_t *options = init_options();
    char *argv[] = {
        "zappy_server", "-n", "team_1", "team-2", "team.3", "-p", "8080"};
    int argc = 7;
    int i = 1;

    handle_teams(options, &i, argc, argv);
    cr_assert_not_null(options->teams, "Teams array should not be NULL");
    cr_assert_str_eq(
        options->teams[0], "team_1", "First team should be 'team_1'");
    cr_assert_str_eq(
        options->teams[1], "team-2", "Second team should be 'team-2'");
    cr_assert_str_eq(
        options->teams[2], "team.3", "Third team should be 'team.3'");
    cr_assert_null(options->teams[3], "Teams array should be null-terminated");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_missing_teams)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-n"};
    int argc = 2;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_teams(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for missing teams");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_empty_team_name)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-n", "", "-p", "8080"};
    int argc = 5;
    int i = 1;

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_teams(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for empty team name");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_already_set)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-n", "team1", "-p", "8080"};
    int argc = 5;
    int i = 1;

    // Pre-set teams
    const char *team_names[] = {"existing_team"};
    options->teams = create_teams_array(team_names, 1);

    // Redirect stderr to avoid output during tests
    int stderr_backup = dup(STDERR_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDERR_FILENO);

    handle_teams(options, &i, argc, argv);
    cr_assert_eq(options->error, true,
        "Error flag should be set to true for already set teams");

    // Restore stderr
    dup2(stderr_backup, STDERR_FILENO);
    close(stderr_backup);
    close(dev_null);

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_next_option_encountered)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-n", "team1", "-p", "8080"};
    int argc = 5;
    int i = 1;

    handle_teams(options, &i, argc, argv);
    cr_assert_not_null(options->teams, "Teams array should not be NULL");
    cr_assert_str_eq(
        options->teams[0], "team1", "First team should be 'team1'");
    cr_assert_null(options->teams[1], "Teams array should be null-terminated");
    cr_assert_eq(i, 2, "Index should be set to the next non-team argument");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}

Test(teams_option, handle_teams_with_long_names)
{
    server_options_t *options = init_options();
    char *argv[] = {"zappy_server", "-n", "very_long_team_name_123456789",
        "another_extremely_long_team_name", "-p", "8080"};
    int argc = 6;
    int i = 1;

    handle_teams(options, &i, argc, argv);
    cr_assert_not_null(options->teams, "Teams array should not be NULL");
    cr_assert_str_eq(options->teams[0], "very_long_team_name_123456789",
        "First team should match long name");
    cr_assert_str_eq(options->teams[1], "another_extremely_long_team_name",
        "Second team should match long name");
    cr_assert_null(options->teams[2], "Teams array should be null-terminated");
    cr_assert_eq(options->error, false, "Error flag should remain false");

    cleanup_server_options(options);
}
