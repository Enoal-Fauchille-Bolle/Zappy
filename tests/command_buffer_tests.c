/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Command Buffer Tests
*/

#include "command_handler/command.h"
#include "command_handler/command_buffer.h"
#include "connection/client.h"
#include "connection/server.h"
#include "constants.h"
#include "options_parser/options.h"
#include "vector.h"
#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Helper function to create a test server options
 *
 * @return server_options_t* Mock server options for testing
 */
static server_options_t *create_test_options(void)
{
    server_options_t *options = malloc(sizeof(server_options_t));

    if (options == NULL)
        return NULL;

    options->port = 4242;
    options->width = 10;
    options->height = 10;
    options->teams = NULL;
    options->clients_nb = 10;
    options->frequency = 100;
    options->help = false;
    options->debug = false;
    options->error = false;

    return options;
}

/**
 * @brief Helper function to create a test server
 *
 * @return server_t* Mock server for testing
 */
static server_t *create_test_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    server_options_t *options = create_test_options();

    if (server == NULL || options == NULL) {
        free(server);
        free(options);
        return NULL;
    }

    server->options = options;
    server->game = NULL;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i] = NULL;
    }

    return server;
}

/**
 * @brief Helper function to cleanup test server
 *
 * @param server Server to cleanup
 */
static void cleanup_test_server(server_t *server)
{
    if (server == NULL)
        return;

    free(server->options);
    free(server);
}

/**
 * @brief Helper function to create a test client
 *
 * @return client_t* Mock client for testing
 */
static client_t *create_test_client(void)
{
    client_t *client = malloc(sizeof(client_t));

    if (client == NULL)
        return NULL;

    client->server = create_test_server();
    client->index = 0;
    client->sockfd = -1;
    client->player = NULL;
    client->command_buffer = vector_new(sizeof(command_t *));
    client->writing_buffer = vector_new(sizeof(char *));
    client->is_gui = false;

    return client;
}

/**
 * @brief Helper function to create a test AI client (with buffer limit)
 *
 * @return client_t* Mock AI client for testing
 */
static client_t *create_test_ai_client(void)
{
    client_t *client = create_test_client();
    if (client) {
        client->is_gui = false;
    }
    return client;
}

/**
 * @brief Helper function to create a test GUI client (no buffer limit)
 *
 * @return client_t* Mock GUI client for testing
 */
static client_t *create_test_gui_client(void)
{
    client_t *client = create_test_client();
    if (client) {
        client->is_gui = true;
    }
    return client;
}

/**
 * @brief Helper function to create a test command
 *
 * @param name Command name
 * @return command_t* Test command structure
 */
static command_t *create_test_command(const char *name)
{
    command_t *command = malloc(sizeof(command_t));

    if (command == NULL)
        return NULL;

    command->name = strdup(name);
    command->argc = 0;
    command->argv = NULL;
    command->tokens = malloc(sizeof(char *) * 2);

    if (command->tokens) {
        command->tokens[0] = strdup(name);
        command->tokens[1] = NULL;
    }

    return command;
}

/**
 * @brief Helper function to cleanup test command
 *
 * @param command Command to cleanup
 */
static void cleanup_test_command(command_t *command)
{
    if (command == NULL)
        return;

    if (command->tokens) {
        for (int i = 0; command->tokens[i] != NULL; i++) {
            free(command->tokens[i]);
        }
        free(command->tokens);
    }

    free(command->name);
    free(command);
}

/**
 * @brief Helper function to get command buffer size
 *
 * @param client Client to check
 * @return size_t Buffer size
 */
static size_t get_command_buffer_size(client_t *client)
{
    const vector_vtable_t *vtable;

    if (client == NULL || client->command_buffer == NULL)
        return 0;

    vtable = vector_get_vtable(client->command_buffer);
    return vtable->size(client->command_buffer);
}

/**
 * @brief Helper function to get command at specific index
 *
 * @param client Client to check
 * @param index Index to get command from
 * @return command_t* Command at index or NULL
 */
static command_t *get_command_at_index(client_t *client, size_t index)
{
    const vector_vtable_t *vtable;
    command_t **command_ptr;

    if (client == NULL || client->command_buffer == NULL)
        return NULL;

    vtable = vector_get_vtable(client->command_buffer);
    if (index >= vtable->size(client->command_buffer))
        return NULL;

    command_ptr = (command_t **)vtable->at(client->command_buffer, index);
    return command_ptr ? *command_ptr : NULL;
}

/**
 * @brief Helper function to cleanup test client
 *
 * @param client Client to cleanup
 */
static void cleanup_test_client(client_t *client)
{
    if (client == NULL)
        return;

    clear_command_buffer(client);
    if (client->command_buffer) {
        vector_destroy(client->command_buffer);
        client->command_buffer = NULL;
    }
    if (client->writing_buffer) {
        vector_destroy(client->writing_buffer);
        client->writing_buffer = NULL;
    }

    cleanup_test_server(client->server);
    free(client);
}

Test(command_buffer, add_command_to_buffer_valid)
{
    client_t *client = create_test_client();
    command_t *command = create_test_command("dummy");

    cr_assert_not_null(client, "Client should not be NULL");
    cr_assert_not_null(command, "Command should not be NULL");

    add_command_to_buffer(client, command);

    cr_assert_eq(get_command_buffer_size(client), 1, "Buffer size should be 1");
    cr_assert_not_null(get_command_at_index(client, 0), "First command slot should not be NULL");
    cr_assert_str_eq(get_command_at_index(client, 0)->name, "dummy",
        "Command name should be 'dummy'");

    cleanup_test_client(client);
}

Test(command_buffer, add_command_to_buffer_null_client)
{
    command_t *command = create_test_command("dummy");

    cr_assert_not_null(command, "Command should not be NULL");

    // This should not crash
    add_command_to_buffer(NULL, command);
}

Test(command_buffer, add_command_to_buffer_null_command)
{
    client_t *client = create_test_client();

    cr_assert_not_null(client, "Client should not be NULL");

    // This should not crash
    add_command_to_buffer(client, NULL);

    // Buffer should be empty
    cr_assert_eq(get_command_buffer_size(client), 0, "Buffer should be empty");

    cleanup_test_client(client);
}

Test(command_buffer, add_command_to_buffer_both_null)
{
    // This should not crash
    add_command_to_buffer(NULL, NULL);
}

Test(command_buffer, add_multiple_commands)
{
    client_t *client = create_test_client();
    command_t *command1 = create_test_command("forward");
    command_t *command2 = create_test_command("left");
    command_t *command3 = create_test_command("right");

    cr_assert_not_null(client, "Client should not be NULL");
    cr_assert_not_null(command1, "Command1 should not be NULL");
    cr_assert_not_null(command2, "Command2 should not be NULL");
    cr_assert_not_null(command3, "Command3 should not be NULL");

    add_command_to_buffer(client, command1);
    add_command_to_buffer(client, command2);
    add_command_to_buffer(client, command3);

    cr_assert_eq(get_command_buffer_size(client), 3, "Buffer size should be 3");
    cr_assert_not_null(get_command_at_index(client, 0), "First command slot should not be NULL");
    cr_assert_not_null(get_command_at_index(client, 1), "Second command slot should not be NULL");
    cr_assert_not_null(get_command_at_index(client, 2), "Third command slot should not be NULL");

    cr_assert_str_eq(get_command_at_index(client, 0)->name, "forward",
        "First command should be 'forward'");
    cr_assert_str_eq(get_command_at_index(client, 1)->name, "left",
        "Second command should be 'left'");
    cr_assert_str_eq(get_command_at_index(client, 2)->name, "right",
        "Third command should be 'right'");

    cleanup_test_client(client);
}

Test(command_buffer, add_command_to_full_buffer)
{
    client_t *client = create_test_ai_client();  // AI client has buffer limit

    cr_assert_not_null(client, "Client should not be NULL");

    // Fill the buffer completely
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        command_t *command = create_test_command("dummy");
        cr_assert_not_null(command, "Command %d should not be NULL", i);
        add_command_to_buffer(client, command);
    }

    // Verify buffer is full
    cr_assert_eq(get_command_buffer_size(client), MAX_COMMAND_BUFFER_SIZE, 
        "Buffer should be full");
    
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        cr_assert_not_null(get_command_at_index(client, i),
            "Command slot %d should not be NULL", i);
        cr_assert_str_eq(get_command_at_index(client, i)->name, "dummy",
            "Command %d should be 'dummy'", i);
    }

    // Try to add one more command (should not crash, but won't be added)
    command_t *extra_command = create_test_command("extra");
    cr_assert_not_null(extra_command, "Extra command should not be NULL");

    add_command_to_buffer(client, extra_command);

    // Buffer should still be full with original commands, extra should be freed
    cr_assert_eq(get_command_buffer_size(client), MAX_COMMAND_BUFFER_SIZE, 
        "Buffer should still be full");
    
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        cr_assert_not_null(get_command_at_index(client, i),
            "Command slot %d should not be NULL", i);
        cr_assert_str_eq(get_command_at_index(client, i)->name, "dummy",
            "Command %d should be 'dummy'", i);
    }

    cleanup_test_client(client);
    // extra_command should have been freed by add_command_to_buffer
}

Test(command_buffer, pop_command_from_buffer_valid)
{
    client_t *client = create_test_client();
    command_t *command = create_test_command("dummy");

    cr_assert_not_null(client, "Client should not be NULL");
    cr_assert_not_null(command, "Command should not be NULL");

    add_command_to_buffer(client, command);

    command_t *popped = pop_command_from_buffer(client);

    cr_assert_not_null(popped, "Popped command should not be NULL");
    cr_assert_str_eq(
        popped->name, "dummy", "Popped command name should be 'dummy'");
    cr_assert_eq(get_command_buffer_size(client), 0, "Buffer should be empty after pop");

    cleanup_test_command(popped);
    cleanup_test_client(client);
}

Test(command_buffer, pop_command_from_buffer_null_client)
{
    command_t *popped = pop_command_from_buffer(NULL);

    cr_assert_null(popped, "Popped command should be NULL for NULL client");
}

Test(command_buffer, pop_command_from_empty_buffer)
{
    client_t *client = create_test_client();

    cr_assert_not_null(client, "Client should not be NULL");

    command_t *popped = pop_command_from_buffer(client);

    cr_assert_null(popped, "Popped command should be NULL for empty buffer");

    cleanup_test_client(client);
}

Test(command_buffer, pop_command_fifo_order)
{
    client_t *client = create_test_client();
    command_t *command1 = create_test_command("first");
    command_t *command2 = create_test_command("second");
    command_t *command3 = create_test_command("third");

    cr_assert_not_null(client, "Client should not be NULL");
    cr_assert_not_null(command1, "Command1 should not be NULL");
    cr_assert_not_null(command2, "Command2 should not be NULL");
    cr_assert_not_null(command3, "Command3 should not be NULL");

    add_command_to_buffer(client, command1);
    add_command_to_buffer(client, command2);
    add_command_to_buffer(client, command3);

    command_t *popped1 = pop_command_from_buffer(client);
    command_t *popped2 = pop_command_from_buffer(client);
    command_t *popped3 = pop_command_from_buffer(client);

    cr_assert_not_null(popped1, "First popped command should not be NULL");
    cr_assert_not_null(popped2, "Second popped command should not be NULL");
    cr_assert_not_null(popped3, "Third popped command should not be NULL");

    cr_assert_str_eq(
        popped1->name, "first", "First popped command should be 'first'");
    cr_assert_str_eq(
        popped2->name, "second", "Second popped command should be 'second'");
    cr_assert_str_eq(
        popped3->name, "third", "Third popped command should be 'third'");

    // Buffer should be empty
    cr_assert_eq(get_command_buffer_size(client), 0, "Buffer should be empty");

    cleanup_test_command(popped1);
    cleanup_test_command(popped2);
    cleanup_test_command(popped3);
    cleanup_test_client(client);
}

Test(command_buffer, pop_command_shifts_remaining)
{
    client_t *client = create_test_client();
    command_t *command1 = create_test_command("first");
    command_t *command2 = create_test_command("second");
    command_t *command3 = create_test_command("third");

    cr_assert_not_null(client, "Client should not be NULL");
    cr_assert_not_null(command1, "Command1 should not be NULL");
    cr_assert_not_null(command2, "Command2 should not be NULL");
    cr_assert_not_null(command3, "Command3 should not be NULL");

    add_command_to_buffer(client, command1);
    add_command_to_buffer(client, command2);
    add_command_to_buffer(client, command3);

    // Pop first command
    command_t *popped = pop_command_from_buffer(client);

    cr_assert_not_null(popped, "Popped command should not be NULL");
    cr_assert_str_eq(
        popped->name, "first", "Popped command should be 'first'");

    // Remaining commands should shift (vector automatically handles this)
    cr_assert_eq(get_command_buffer_size(client), 2, "Buffer should have 2 commands left");
    cr_assert_not_null(get_command_at_index(client, 0), "First slot should contain second command");
    cr_assert_not_null(get_command_at_index(client, 1), "Second slot should contain third command");
    cr_assert_null(get_command_at_index(client, 2), "Third slot should be NULL (out of bounds)");

    cr_assert_str_eq(get_command_at_index(client, 0)->name, "second",
        "First slot should be 'second'");
    cr_assert_str_eq(get_command_at_index(client, 1)->name, "third",
        "Second slot should be 'third'");

    cleanup_test_command(popped);
    cleanup_test_client(client);
}

Test(command_buffer, mixed_add_pop_operations)
{
    client_t *client = create_test_client();

    cr_assert_not_null(client, "Client should not be NULL");

    // Add some commands
    command_t *cmd1 = create_test_command("cmd1");
    command_t *cmd2 = create_test_command("cmd2");
    add_command_to_buffer(client, cmd1);
    add_command_to_buffer(client, cmd2);

    // Pop one
    command_t *popped1 = pop_command_from_buffer(client);
    cr_assert_not_null(popped1, "First popped command should not be NULL");
    cr_assert_str_eq(popped1->name, "cmd1", "First popped should be 'cmd1'");

    // Add more
    command_t *cmd3 = create_test_command("cmd3");
    command_t *cmd4 = create_test_command("cmd4");
    add_command_to_buffer(client, cmd3);
    add_command_to_buffer(client, cmd4);

    // Pop remaining in order
    command_t *popped2 = pop_command_from_buffer(client);
    command_t *popped3 = pop_command_from_buffer(client);
    command_t *popped4 = pop_command_from_buffer(client);

    cr_assert_not_null(popped2, "Second popped command should not be NULL");
    cr_assert_not_null(popped3, "Third popped command should not be NULL");
    cr_assert_not_null(popped4, "Fourth popped command should not be NULL");

    cr_assert_str_eq(popped2->name, "cmd2", "Second popped should be 'cmd2'");
    cr_assert_str_eq(popped3->name, "cmd3", "Third popped should be 'cmd3'");
    cr_assert_str_eq(popped4->name, "cmd4", "Fourth popped should be 'cmd4'");

    // Buffer should be empty
    command_t *empty_pop = pop_command_from_buffer(client);
    cr_assert_null(empty_pop, "Pop from empty buffer should return NULL");

    cleanup_test_command(popped1);
    cleanup_test_command(popped2);
    cleanup_test_command(popped3);
    cleanup_test_command(popped4);
    cleanup_test_client(client);
}

Test(command_buffer, fill_and_empty_full_buffer)
{
    client_t *client = create_test_client();
    command_t *commands[MAX_COMMAND_BUFFER_SIZE];

    cr_assert_not_null(client, "Client should not be NULL");

    // Fill buffer completely
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        commands[i] = create_test_command("test");
        cr_assert_not_null(commands[i], "Command %d should not be NULL", i);
        add_command_to_buffer(client, commands[i]);
    }

    // Verify buffer is full
    cr_assert_eq(get_command_buffer_size(client), MAX_COMMAND_BUFFER_SIZE, 
        "Buffer should be full");
    
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        cr_assert_not_null(get_command_at_index(client, i),
            "Buffer slot %d should not be NULL", i);
    }

    // Empty buffer completely
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE; i++) {
        command_t *popped = pop_command_from_buffer(client);
        cr_assert_not_null(popped, "Popped command %d should not be NULL", i);
        cr_assert_str_eq(
            popped->name, "test", "Popped command %d should be 'test'", i);
        cleanup_test_command(popped);
    }

    // Verify buffer is empty
    cr_assert_eq(get_command_buffer_size(client), 0, "Buffer should be empty");

    // One more pop should return NULL
    command_t *extra_pop = pop_command_from_buffer(client);
    cr_assert_null(extra_pop, "Extra pop should return NULL");

    cleanup_test_client(client);
}

Test(command_buffer, gui_client_no_buffer_limit)
{
    client_t *client = create_test_gui_client();  // GUI client has no buffer limit

    cr_assert_not_null(client, "Client should not be NULL");

    // Try to add more commands than the AI limit (this should work for GUI)
    for (int i = 0; i < MAX_COMMAND_BUFFER_SIZE + 5; i++) {
        command_t *command = create_test_command("gui_command");
        cr_assert_not_null(command, "Command %d should not be NULL", i);
        add_command_to_buffer(client, command);
    }

    // Verify GUI client can exceed the AI buffer limit
    cr_assert_eq(get_command_buffer_size(client), MAX_COMMAND_BUFFER_SIZE + 5, 
        "GUI client should be able to exceed AI buffer limit");

    cleanup_test_client(client);
}

Test(command_buffer, is_command_buffer_empty_test)
{
    client_t *client = create_test_client();

    cr_assert_not_null(client, "Client should not be NULL");

    // Buffer should start empty
    cr_assert(is_command_buffer_empty(client), "Buffer should start empty");

    // Add a command
    command_t *command = create_test_command("test");
    add_command_to_buffer(client, command);

    // Buffer should not be empty
    cr_assert(!is_command_buffer_empty(client), "Buffer should not be empty");

    // Pop the command
    command_t *popped = pop_command_from_buffer(client);
    cleanup_test_command(popped);

    // Buffer should be empty again
    cr_assert(is_command_buffer_empty(client), "Buffer should be empty again");

    cleanup_test_client(client);
}
