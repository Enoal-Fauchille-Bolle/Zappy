/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** player
*/
// clang-format Language: C

#ifndef PLAYER_H_
    #define PLAYER_H_

    #include "connection/client.h"
    #include "game/game.h"
    #include "map/coordinates.h"
    #include "map/resources.h"
    #include "map/tile.h"
    #include "team/egg/egg.h"
    #include <stddef.h>

typedef unsigned int level_t;
typedef unsigned int tick_t;
typedef struct team_s team_t;
typedef struct egg_s egg_t;

typedef size_t player_id_t;
typedef size_t egg_id_t;

typedef struct player_s {
    player_id_t id;             // Unique identifier for the player
    pos_t pos;                  // Player's position on the map
    orientation_t orientation;  // Player's orientation
    level_t level;              // Player's level, starting at 1
    inventory_t inventory;      // Player's inventory, indexed by resource_t
    tick_t tick_cooldown;       // If 0, player can act
    size_t hunger_cooldown;     // If 0, player NEEDS to eat or die
    bool doing_action;          // If player is currently performing an action
    bool in_incantation;        // If player is in an incantation
    team_t *team;               // Pointer to the team this player belongs to
    client_t *client;           // Pointer to the client managing this player
} player_t;

player_t *create_player(
    const pos_t pos, const size_t id, team_t *team, client_t *client);
void destroy_player(player_t *player);

// Player actions
void turn_player_left(player_t *player);
void turn_player_right(player_t *player);
void move_player_forward(player_t *player, map_t *map);
egg_t *lay_egg(player_t *player, map_t *map);
char *look(player_t *player, map_t *map);
char *check_inventory(player_t *player);
bool take_resource(player_t *player, map_t *map, resource_t resource);
bool set_resource(player_t *player, map_t *map, resource_t resource);
bool eject(player_t *player, map_t *map);
direction_t get_broadcast_direction(player_t *listening_player,
    player_t *broadcasting_player, const map_t *map);

void add_player_to_tile(tile_t *tile, player_t *player);
void remove_player_from_tile(tile_t *tile, player_t *player);
void add_player_to_map(map_t *map, player_t *player);
void remove_player_from_map(map_t *map, player_t *player);

#endif /* !PLAYER_H_ */
