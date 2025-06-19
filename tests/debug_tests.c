/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** debug_tests
*/

#include "debug.h"
#include "debug_categories.h"
#include <criterion/criterion.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

Test(debug, info, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_info(debug_enabled, "This is an info message: %d\n", 42);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [ℹ️ ] This is an info message: 42\n
    cr_assert(strstr(buffer, "[ℹ️ ] This is an info message: 42\n") != NULL,
        "Debug info message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, warning, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_warning(debug_enabled, "This is a warning message: %d\n", 42);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [⚠️ ] This is a warning message: 42\n
    cr_assert(strstr(buffer, "[⚠️ ] This is a warning message: 42\n") != NULL,
        "Debug warning message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, error, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_error(debug_enabled, "This is an error message: %d\n", 42);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [❌] This is an error message: 42\n
    cr_assert(strstr(buffer, "[❌] This is an error message: 42\n") != NULL,
        "Debug error message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, server, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_server(debug_enabled, "Server started on port: %d\n", 3000);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [🖥️ ] Server started on port: 3000\n
    cr_assert(strstr(buffer, "[🖥️ ] Server started on port: 3000\n") != NULL,
        "Debug server message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, connection, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_conn(debug_enabled, "Client connected from IP: %s\n", "127.0.0.1");
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [🔗] Client connected from IP: 127.0.0.1\n
    cr_assert(strstr(buffer, "[🔗] Client connected from IP: 127.0.0.1\n") != NULL,
        "Debug connection message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, command, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_cmd(debug_enabled, "Executing command: %s\n", "Forward");
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [⚡] Executing command: Forward\n
    cr_assert(strstr(buffer, "[⚡] Executing command: Forward\n") != NULL,
        "Debug command message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, game, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_game(debug_enabled, "Game tick %d\n", 42);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [🎮] Game tick 42\n
    cr_assert(strstr(buffer, "[🎮] Game tick 42\n") != NULL,
        "Debug game message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, player, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_player(debug_enabled, "Player %d leveled up to level %d\n", 1, 2);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [👤] Player 1 leveled up to level 2\n
    cr_assert(strstr(buffer, "[👤] Player 1 leveled up to level 2\n") != NULL,
        "Debug player message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, map, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_map(debug_enabled, "Map created with dimensions %d x %d\n", 10, 10);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [🌐] Map created with dimensions 10 x 10\n
    cr_assert(strstr(buffer, "[🌐] Map created with dimensions 10 x 10\n") != NULL,
        "Debug map message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, resource, .timeout = 2)
{
    bool debug_enabled = true;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_resource(debug_enabled, "Resource %s spawned at (%d, %d)\n", "food", 5, 3);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // Check if output contains the expected pattern
    // Format should be: [HH:MM:SS.mmm] [💎] Resource food spawned at (5, 3)\n
    cr_assert(strstr(buffer, "[💎] Resource food spawned at (5, 3)\n") != NULL,
        "Debug resource message should contain expected text with emoji prefix");

    // Check timestamp format [HH:MM:SS.mmm] at the beginning
    regex_t regex;
    int ret = regcomp(
        &regex, "^\\[[0-9]{2}:[0-9]{2}:[0-9]{2}\\.[0-9]{3}\\]", REG_EXTENDED);
    cr_assert_eq(ret, 0, "Failed to compile regex");

    ret = regexec(&regex, buffer, 0, NULL, 0);
    cr_assert_eq(
        ret, 0, "Output should start with timestamp format [HH:MM:SS.mmm]");

    regfree(&regex);
}

Test(debug, disabled, .timeout = 2)
{
    bool debug_enabled = false;

    // Redirect stdout to a temporary file
    FILE *temp = tmpfile();
    cr_assert_not_null(temp, "Failed to create temporary file");

    // Backup original stdout
    FILE *original_stdout = stdout;
    stdout = temp;

    debug_info(debug_enabled, "This message should not appear: %d\n", 42);
    debug_warning(debug_enabled, "This warning should not appear: %d\n", 42);
    debug_error(debug_enabled, "This error should not appear: %d\n", 42);
    debug_server(debug_enabled, "This server message should not appear: %d\n", 42);
    fflush(stdout);

    // Restore stdout
    stdout = original_stdout;

    // Read from temporary file
    rewind(temp);
    char buffer[1024];
    size_t read_size = fread(buffer, 1, sizeof(buffer) - 1, temp);
    buffer[read_size] = '\0';
    fclose(temp);

    // When debug is disabled, no output should be produced
    cr_assert_eq(read_size, 0, "No output should be produced when debug is disabled");
}
