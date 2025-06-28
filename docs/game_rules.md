# Zappy Game Rules

## Table of Contents

- [Overview](#overview)
- [How the Game Works](#how-the-game-works)
  - [The World](#the-world)
  - [Players & Teams](#players--teams)
  - [Actions](#actions)
- [Game Mechanics](#game-mechanics)
  - [Survival](#survival)
  - [Vision](#vision)
  - [Elevation System](#elevation-system)
    - [Elevation Requirements Table](#elevation-requirements-table)
  - [Communication](#communication)
- [Game Goal & Win Condition](#game-goal--win-condition)
  - [The Goal](#the-goal)
  - [Win Condition](#win-condition)
- [Additional Rules & Notes](#additional-rules--notes)
- [Summary Table](#summary-table)
- [References](#references)

## Overview

Zappy is a real-time, multiplayer programming game inspired by "The Hitchhiker's Guide to the Galaxy." Players control Trantorian inhabitants competing in teams to achieve spiritual elevation by collecting resources, cooperating, and evolving together on a spherical world named Trantor. The game features both AI and human participants, but is primarily designed as a programming challenge for autonomous agents (AIs).

---

## How the Game Works

### The World

- **Map:** The world is a 2D grid with wrap-around edges (toroidal/spherical). If a player moves off one edge, they appear on the opposite side.
- **Tiles:** Each tile can contain resources (stones and food), eggs, and players.
- **Resources:** There are six types of stones (`linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`) and food. Stones are used for elevation; food is required for survival.

### Players & Teams

- **Teams:** Each player belongs to a team. Multiple teams compete simultaneously.
- **Players:** Controlled by AI clients, each player acts autonomously and has:
  - Position and orientation on the map
  - Level (starting at 1)
  - Inventory (stores collected resources)
  - Vision (field of view expands as the player’s level increases)
  - Limited lifespan, requiring food to survive

- **Eggs:** Players can lay eggs to allow new team members to join later.

### Actions

- **Movement:** Players can move forward, turn right/left, and explore the map.
- **Resource Management:** Players collect and drop resources, and must eat food to avoid starvation.
- **Communication:** Players can broadcast messages to coordinate or mislead.
- **Reproduction:** Players use the `fork` action to lay eggs, which can hatch into new players for their team.
- **Elevation (Leveling Up):** Players perform rituals in groups to evolve to the next level (see below).

---

## Game Mechanics

### Survival

- **Food:** Each player needs food to live. Food is consumed automatically over time. If a player runs out of food, they die.
- **Resource Spawning:** The server periodically spawns food and stones at random locations according to predefined densities.

### Vision

- Players can "look" to see tiles ahead. The field of view grows as the player’s level increases.

### Elevation System

- **Leveling Up:** To increase their level, players must gather on the same tile and perform an elevation ritual, consuming specific resources and requiring a specific number of players at the same level.

#### Elevation Requirements Table

| Level | Players Needed | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|---------------|----------|-----------|-------|----------|--------|----------|
| 1→2   | 1             | 1        | 0         | 0     | 0        | 0      | 0        |
| 2→3   | 2             | 1        | 1         | 1     | 0        | 0      | 0        |
| 3→4   | 2             | 2        | 0         | 1     | 0        | 2      | 0        |
| 4→5   | 4             | 1        | 1         | 2     | 0        | 1      | 0        |
| 5→6   | 4             | 1        | 2         | 1     | 3        | 0      | 0        |
| 6→7   | 6             | 1        | 2         | 3     | 0        | 1      | 0        |
| 7→8   | 6             | 2        | 2         | 2     | 2        | 2      | 1        |

- **Ritual:** All required players must be present on the same tile, at the same level, and the correct resources placed. After a delay, if successful, all involved players level up.

### Communication

- **Broadcast:** Players can send messages to others, which are directional (the receiver knows the direction from which it came).
- **Eject:** Players can push others off their tile.

---

## Game Goal & Win Condition

### The Goal

- Work as a team to survive, gather resources, coordinate through communication, and elevate as many team members as possible.
- Efficient resource management and strategic cooperation are key.

### Win Condition

- **Victory:** The first team to have **at least 6 players reach level 8** wins the game.
- The server announces the winning team to all clients, ending the game.

---

## Additional Rules & Notes

- **Player Capacity:** Each team has a maximum number of players (set at game start). New players can only join if there is room or if an egg is available.
- **Time Management:** All actions take a specific time to execute, managed by the server’s clock frequency (`freq`).
- **Autonomous Play:** AI clients are expected to play without human intervention.
- **No Direct Combat:** The game is focused on resource competition, cooperation, and strategy rather than direct combat.

---

## Summary Table

| Aspect          | Details                                                        |
|-----------------|----------------------------------------------------------------|
| Map             | 2D grid, spherical (wraps at edges)                            |
| Teams           | Compete to evolve 6 players to level 8                         |
| Elevation       | Requires teamwork and resource combinations                    |
| Survival        | Players must eat food periodically                             |
| Communication   | Broadcasts, ejection, and cooperative rituals                  |
| Victory         | First team with 6 level-8 members wins                         |
| Death           | Players die if food runs out                                   |

---

## References

- For full technical details, see [README.md](../README.md) and [project subject PDF](B-YEP-400_zappy.pdf).