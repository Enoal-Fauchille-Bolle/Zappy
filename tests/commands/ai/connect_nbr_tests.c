/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Connect Nbr Command Tests
*/

#include "command_handler/ai_commands.h"
#include "command_handler/command.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "constants.h"
#include "game/game.h"
#include "game/incantation.h"
#include "map/coordinates.h"
#include "map/map.h"
#include "map/resources.h"
#include "map/tile.h"
#include "options_parser/options.h"
#include "team/player/player.h"
#include "vector.h"
#include <criterion/criterion.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static server_t *create_test_server(void)
{
    server_t *server = malloc(sizeof(server_t));
    if (!server)
        return NULL;

    server->options = malloc(sizeof(server_options_t));
    if (!server->options) {
        free(server);
        return NULL;
    }

    server->options->debug = false;
    server->options->port = 4242;
    server->options->width = 10;
    server->options->height = 10;
    server->options->frequency = 100;
    server->game = NULL;

    // Initialize clients array to NULL
    for (int i = 0; i < MAX_CLIENTS; i++) {
        server->clients[i] = NULL;
    }
    return server;
}

static void destroy_test_server(server_t *server)
{
    if (!server)
        return;
    if (server->options)
        free(server->options);
    free(server);
}

static client_t *create_test_client(server_t *server)
{
    client_t *client = malloc(sizeof(client_t));
    if (!client)
        return NULL;

    client->sockfd = -1;
    client->server = server;
    client->is_gui = false;
    client->player = NULL;
    client->index = 0;
    client->command_buffer = vector_new(sizeof(command_t *));
    client->writing_buffer = vector_new(sizeof(char *));
    return client;
}

static void destroy_test_client(client_t *client)
{
    if (!client)
        return;
    if (client->writing_buffer) {
        clear_writing_buffer(client);
        vector_destroy(client->writing_buffer);
        client->writing_buffer = NULL;
    }
    if (client->command_buffer) {
        vector_destroy(client->command_buffer);
        client->command_buffer = NULL;
    }
    free(client);
}

static team_t *create_test_team(const char *name, game_t *game)
{
    team_t *team = malloc(sizeof(team_t));
    if (!team)
        return NULL;

    team->name = strdup(name);
    team->players = vector_new(sizeof(player_t *));
    team->eggs = vector_new(sizeof(egg_t *));
    team->game = game;
    return team;
}

static void destroy_test_team(team_t *team)
{
    if (!team)
        return;
    if (team->name)
        free(team->name);
    if (team->players)
        vector_destroy(team->players);
    if (team->eggs)
        vector_destroy(team->eggs);
    free(team);
}

static game_t *create_test_game(void)
{
    server_t *server = create_test_server();
    if (!server)
        return NULL;

    game_t *game = malloc(sizeof(game_t));
    if (!game) {
        destroy_test_server(server);
        return NULL;
    }

    game->server = server;
    game->map = create_map(10, 10, server);
    game->incantations = vector_new(sizeof(incantation_t *));
    game->next_player_id = 1;
    game->next_egg_id = 1;
    game->tick_rate = 100;
    game->game_tick = 0;

    // Create teams array (NULL-terminated)
    game->teams = malloc(sizeof(team_t *) * 3);
    game->teams[0] = create_test_team("team1", game);
    game->teams[1] = create_test_team("team2", game);
    game->teams[2] = NULL;

    server->game = game;
    return game;
}

static void destroy_test_game(game_t *game)
{
    if (!game)
        return;

    // Clean up incantations vector carefully - but don't destroy the
    // incantations themselves as they might be destroyed by update_incantation
    if (game->incantations) {
        vector_destroy(game->incantations);
    }

    // Destroy map after cleaning up incantations
    if (game->map)
        destroy_map(game->map);

    // Clean up teams
    if (game->teams) {
        for (int i = 0; game->teams[i]; i++) {
            destroy_test_team(game->teams[i]);
        }
        free(game->teams);
    }

    // Clean up server last
    if (game->server)
        destroy_test_server(game->server);

    free(game);
}

static player_t *create_test_player(game_t *game, pos_t pos, level_t level)
{
    client_t *client = create_test_client(game->server);
    if (!client)
        return NULL;

    player_t *player = malloc(sizeof(player_t));
    if (!player) {
        destroy_test_client(client);
        return NULL;
    }

    // Initialize player
    player->id = game->next_player_id++;
    player->pos = pos;
    player->orientation = NORTH;
    player->level = level;
    player->tick_cooldown = 0;
    player->hunger_cooldown = 126;
    player->doing_action = false;
    player->in_incantation = false;
    player->team = game->teams[0];
    player->client = client;

    // Initialize inventory to zero
    for (resource_t r = FOOD; r < RESOURCE_COUNT; r++) {
        player->inventory[r] = 0;
    }
    player->inventory[FOOD] = 10;      // Give some food

    client->player = player;

    // Add player to the tile at the specified position
    tile_t *tile = get_tile(game->map, pos);
    if (tile) {
        const vector_vtable_t *vtable = vector_get_vtable(tile->players);
        vtable->push_back(tile->players, &player);
    }

    return player;
}

static void destroy_test_player(player_t *player)
{
    if (!player)
        return;
    if (player->client) {
        destroy_test_client(player->client);
        player->client = NULL;
    }
    free(player);
}

/**
 * @brief Helper function to create pipe for testing socket writes
 */
static int create_test_pipe(int *write_fd)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
        return -1;

    *write_fd = pipefd[1];
    return pipefd[0];      // read end
}

/**
 * @brief Helper function to read from pipe with timeout
 */
static char *read_from_pipe(int read_fd)
{
    char buffer[256];
    ssize_t bytes_read;
    fd_set readfds;
    struct timeval timeout;

    // Set up timeout for reading
    FD_ZERO(&readfds);
    FD_SET(read_fd, &readfds);
    timeout.tv_sec = 1;  // 1 second timeout
    timeout.tv_usec = 0;

    // Wait for data to be available
    int result = select(read_fd + 1, &readfds, NULL, NULL, &timeout);
    if (result <= 0) {
        return NULL; // Timeout or error
    }

    bytes_read = read(read_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
        return NULL;

    buffer[bytes_read] = '\0';
    return strdup(buffer);
}

/**
 * @brief Helper function to flush writing buffer for a client
 */
static void flush_client_writing_buffer(client_t *client)
{
    // We need to manually process the writing buffer since we're not running
    // the main server loop. Create a temporary server structure and call the
    // writing buffer handler.
    server_t temp_server;
    temp_server.clients[0] = client;
    for (int i = 1; i < MAX_CLIENTS; i++) {
        temp_server.clients[i] = NULL;
    }
    
    handle_clients_writing_buffer(&temp_server);
}

Test(connect_nbr_command, connect_nbr_basic, .timeout = 2)
{
    server_t *server = create_test_server();
    game_t *game = create_test_game();
    server->game = game;
    client_t *client = create_test_client(server);
    client->player = create_test_player(server->game, (pos_t){2, 2}, 1);
    command_t cmd = {0};
    cmd.name = strdup("Connect_nbr");
    cmd.argc = 0;
    cmd.argv = NULL;
    int read_fd, write_fd;
    read_fd = create_test_pipe(&write_fd);
    cr_assert_neq(read_fd, -1, "Pipe creation should succeed");
    client->sockfd = write_fd;

    connect_nbr_command(client, &cmd);

    flush_client_writing_buffer(client);
    close(write_fd);

    char *response = read_from_pipe(read_fd);
    cr_assert_not_null(response, "Response should not be NULL");
    cr_assert_str_eq(response, "0\n", "connect_nbr should return '0\\n' for a new team with no eggs");

    close(read_fd);
    free(response);
    destroy_test_player(client->player);
    destroy_test_client(client);
    destroy_test_game(game);
    destroy_test_server(server);
}
