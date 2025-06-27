/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tick Management
*/

#include "command_handler/command.h"
#include "command_handler/command_buffer.h"
#include "command_handler/command_executor.h"
#include "command_handler/command_factory.h"
#include "command_handler/gui_commands.h"
#include "connection/client.h"
#include "connection/server.h"
#include "connection/writing_buffer.h"
#include "constants.h"
#include "debug_categories.h"
#include "game/game.h"
#include "game/game_constants.h"
#include "game/game_state.h"
#include "game/incantation.h"
#include "map/resources.h"
#include "options_parser/options.h"
#include "team/egg/egg.h"
#include "team/player/player.h"
#include "team/team.h"
#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * @brief Handle player starvation by checking their food inventory
 *
 * This function checks if the player has food available. If not, it marks the
 * player as dead and removes them from the game. If the player has food, it
 * decrements their food inventory and resets the hunger cooldown.
 *
 * @param player Pointer to the player structure to check for starvation
 * @return true if the player has starved and is removed, false otherwise
 */
static bool handle_player_starvation(player_t *player)
{
    if (player->hunger_cooldown > 0) {
        player->hunger_cooldown--;
        return false;
    }
    if (player->inventory[FOOD] == 0) {
        debug_player(player->client->server->options->debug,
            "Player %zu (Client %d) has starved\n", player->id,
            player->client->index);
        pdi_event(player);
        remove_client(player->client->server, player->client->index + 2);
        return true;
    }
    player->inventory[FOOD]--;
    pin_event(player);
    debug_player(player->client->server->options->debug,
        "Player %zu has eaten, remaining food: %zu\n", player->id,
        player->inventory[FOOD]);
    player->hunger_cooldown = GAME_TICK_FOOD_COOLDOWN;
    return false;
}

/**
 * @brief Update the tick cooldown for a player
 *
 * This function decrements the player's tick cooldown if it is greater than 0.
 * If the cooldown reaches 0, the player can act again.
 *
 * @param player Pointer to the player structure to update
 */
static void update_player_tick_cooldown(player_t *player)
{
    if (player->tick_cooldown > 0)
        player->tick_cooldown--;
    if (player->doing_action && player->tick_cooldown == 0) {
        debug_cmd(player->client->server->options->debug,
            "Player %zu is ready to act again\n", player->id);
        player->doing_action = false;
    }
}

/**
 * @brief Update the tick cooldown for all players in a team
 *
 * This function iterates through all players in a team and updates their
 * tick cooldown. It also checks for player starvation and handles it if
 * necessary.
 *
 * @param team Pointer to the team structure containing players
 */
static void update_players_team_ticks(team_t *team)
{
    const vector_vtable_t *vtable = vector_get_vtable(team->players);
    player_t *player = NULL;
    size_t i = 0;

    while (i < vtable->size(team->players)) {
        player = *(player_t **)vtable->at(team->players, i);
        if (player == NULL) {
            i++;
            continue;
        }
        update_player_tick_cooldown(player);
        if (player->client->server->game->game_state == GAME_RUNNING &&
            handle_player_starvation(player)) {
            continue;
        }
        i++;
    }
}

/**
 * @brief Update the tick cooldown for all players in the game
 *
 * This function iterates through all teams in the game and updates the tick
 * cooldown for each player. It also checks for player starvation and handles
 * it accordingly.
 *
 * @param game Pointer to the game structure containing teams and players
 */
static void update_players_ticks(game_t *game)
{
    for (int i = 0; game->teams[i] != NULL; i++) {
        update_players_team_ticks(game->teams[i]);
    }
}

/**
 * @brief Execute a single command for a player
 *
 * This function pops a command from the player's command buffer, executes it,
 * and destroys the command. It also handles debug logging for the executed
 * command.
 *
 * @param player Pointer to the player structure to execute command for
 * @return true if a command was executed, false if no command was available
 */
static bool execute_player_command(player_t *player)
{
    command_t *command = NULL;
    char *command_name = NULL;

    command = pop_command_from_buffer(player->client);
    if (command == NULL)
        return false;
    command_name = command->name ? strdup(command->name) : NULL;
    debug_cmd(player->client->server->options->debug,
        "Player %zu: '%s' command executed\n", player->id,
        command_name ? command_name : "unknown");
    player->doing_action = true;
    execute_command(player->client, command);
    destroy_command(command);
    if (command_name) {
        free(command_name);
        command_name = NULL;
    }
    return true;
}

/**
 * @brief Read and execute command buffer for a single player
 *
 * This function checks if the player is ready to act (i.e., their tick
 * cooldown is 0) and if the client and server pointers are valid. It then
 * executes commands from the player's command buffer while the player can act.
 *
 * @param player Pointer to the player structure to read commands from
 */
static void read_player_command_buffer(player_t *player)
{
    while (player->tick_cooldown == 0) {
        if (player->client == NULL || player->client->server == NULL ||
            player->client->server->options == NULL) {
            fprintf(stderr, "Invalid player or client pointer\n");
            return;
        }
        if (!execute_player_command(player))
            return;
    }
}

/**
 * @brief Read and execute command buffers for all players in the game
 *
 * This function iterates through all teams and players, reading their command
 * buffers and executing the commands if the player is ready to act.
 *
 * @param game Pointer to the game structure containing teams and players
 */
static void read_players_command_buffer(game_t *game)
{
    const vector_vtable_t *vtable = NULL;
    player_t *player = NULL;

    for (int i = 0; game->teams[i] != NULL; i++) {
        vtable = vector_get_vtable(game->teams[i]->players);
        for (size_t j = 0; j < vtable->size(game->teams[i]->players); j++) {
            player = *(player_t **)vtable->at(game->teams[i]->players, j);
            read_player_command_buffer(player);
        }
    }
}

/**
 * @brief Read and execute command buffers for all GUI clients in the server
 *
 * This function iterates through all clients in the server, checking if they
 * are GUI clients. If they are, it pops a command from their command buffer,
 * executes it, and destroys the command.
 *
 * @param server Pointer to the server structure containing clients
 */
static void read_guis_command_buffer(server_t *server)
{
    client_t *client = NULL;
    command_t *command = NULL;

    for (int i = 2; i < MAX_CLIENTS + 2; i++) {
        if (server->clients[i - 2] == NULL || !server->clients[i - 2]->is_gui)
            continue;
        client = server->clients[i - 2];
        command = pop_command_from_buffer(client);
        if (command == NULL)
            continue;
        debug_cmd(server->options->debug,
            "GUI Client %d: '%s' command executed\n", client->index,
            command->name);
        execute_command(client, command);
        destroy_command(command);
    }
}

/**
 * @brief Update the incantation by decrementing ticks and completing it if
 * necessary
 *
 * This function iterates through all incantations in the game, updating each
 * one by decrementing its ticks left. If the ticks reach zero, it completes
 * the incantation and removes it from the vector.
 *
 * @param game Pointer to the game structure containing incantations vector
 */
static void update_incantations(game_t *game)
{
    const vector_vtable_t *vtable = vector_get_vtable(game->incantations);
    incantation_t *incantation = NULL;

    for (size_t i = vtable->size(game->incantations); i > 0; i--) {
        incantation = *(incantation_t **)vtable->at(game->incantations, i - 1);
        if (incantation == NULL) {
            continue;
        }
        update_incantation(incantation, game);
    }
}

/**
 * @brief Advances the game tick counter
 *
 * This function increments the game tick counter, which is used to track
 * the progression of the game state over time.
 *
 * @param game Pointer to the game structure containing the current tick count
 */
void game_tick(game_t *game, server_options_t *options)
{
    if (game->game_tick % GAME_TICK_DEBUG_INTERVAL == 0) {
        debug_game(options->debug, "Game tick %u\n", game->game_tick);
    }
    update_players_ticks(game);
    update_incantations(game);
    read_players_command_buffer(game);
    read_guis_command_buffer(game->server);
    if (game->game_tick % GAME_RESOURCE_SPAWN_INTERVAL == 0) {
        spread_resources(game->map, options->debug);
    }
    for (int i = 0; game->teams[i] != NULL; i++) {
        spawn_min_eggs(
            game->map, game->teams[i], options->clients_nb, options->debug);
    }
    handle_clients_writing_buffer(game->server);
    game->game_tick++;
}
