# Zappy Communication Protocol

## Table of Contents

- [Overview](#overview)
- [General Protocol Guidelines](#general-protocol-guidelines)
  - [TCP Sockets](#tcp-sockets)
  - [Message Format](#message-format)
  - [Character Encoding](#character-encoding)
  - [Connection Management](#connection-management)
  - [Error Handling](#error-handling)
- [Authentication & Handshake](#authentication--handshake)
  - [Initial Handshake](#initial-handshake)
- [AI ↔️ Server Protocol](#ai-️-server-protocol)
  - [Authentication](#authentication)
  - [Command Structure](#command-structure)
  - [AI-Initiated Commands](#ai-initiated-commands)
  - [Server-Initiated Commands (AI)](#server-initiated-commands-ai)
- [GUI ↔️ Server Protocol](#gui-️-server-protocol)
  - [Authentication](#authentication-1)
  - [Command Structure](#command-structure-1)
  - [GUI-Initiated Commands](#gui-initiated-commands)
  - [Server-Initiated Commands (GUI)](#server-initiated-commands-gui)
- [Examples](#examples)
  - [AI Client Example Session](#ai-client-example-session)
  - [GUI Client Example Session](#gui-client-example-session)
- [Additional Notes](#additional-notes)
- [References](#references)

## Overview

Zappy is a distributed multiplayer game consisting of three main components:

- **Server** (C): Orchestrates the game world, player actions, and communication.
- **Graphical Client (GUI)** (C++): Connects to the server for real-time visualization and interaction.
- **AI Client** (any language): Autonomous agents that play the game by connecting to and interacting with the server.

All communication between components occurs over TCP sockets using a line-based, human-readable protocol. This document outlines the communication protocol, connection lifecycle, message structure, command sets, and best practices for robust interaction.

---

## General Protocol Guidelines

### TCP Sockets

- All components communicate via TCP/IP.
- The server listens on a configurable port for both AI and GUI clients.
- Each client opens a direct TCP connection to the server.

### Message Format

- **Line-based** protocol: Each message is a single line terminated by a newline (`\n`).
- **Plain text**: Messages are composed of ASCII characters (UTF-8 safe).
- **Commands**: Each line represents one command or response.
- **No binary data**: All data is transmitted as readable text.

#### Example

```msg
Forward\n
```

```msg
WELCOME\n
```

### Character Encoding

- All messages must use UTF-8 encoding.
- Non-ASCII characters should be avoided except in player/team names, which must be UTF-8 safe and validated by the server.

### Connection Management

- Each connection is stateful and persistent until closed by either party.
- Clients must reconnect and re-authenticate after a disconnect.
- The server may close connections on protocol violations, timeouts, or capacity limits.

### Error Handling

- Invalid commands or malformed messages result in an `ko` (for AI) or error message (GUI) response.
- The server may disconnect clients that repeatedly send invalid input.
- Clients should gracefully handle server-initiated disconnects and unexpected errors.

---

## Authentication & Handshake

### Initial Handshake

**1. Upon connecting:**

- All clients receive the message:

  ```msg
  WELCOME\n
  ```

**2. Authentication phase:**

- **AI Client:** Must send the team name as the very first message.

  ```msg
  <team_name>\n
  ```

- **GUI Client:** Must send the string `GRAPHIC`.

  ```msg
  GRAPHIC\n
  ```

**3. Server responds:***

- If accepted:
  - AI: Sends map size and client slot info.
  - GUI: Sends the full world state (see below).
- If rejected (full teams, unknown team, etc.), the server closes the connection.

---

## AI ↔️ Server Protocol

### Authentication

1. **AI sends:** `<team_name>`
2. **Server responds:**
   - Number of remaining slots for the team
   - Map size

### Command Structure

- Each AI command is sent as a single line of text.
- Commands are **case-sensitive** and must match protocol specification.
- Arguments are separated by spaces.

#### Example

```msg
Forward\n
```

```msg
Set linemate\n
```

### AI-Initiated Commands

| Command         | Description                                    |
|-----------------|------------------------------------------------|
| Forward         | Move player forward                            |
| Right           | Turn right                                     |
| Left            | Turn left                                      |
| Look            | Get visible tiles                              |
| Inventory       | Get inventory                                  |
| Broadcast \<msg> | Broadcast message to all players               |
| Connect_nbr     | Get number of free slots for team              |
| Fork            | Lay an egg (for reproduction)                  |
| Eject           | Push other players off the tile                |
| Take \<object>   | Pick up an object (e.g., Take food)            |
| Set \<object>    | Place an object from inventory onto the tile   |
| Incantation     | Start elevation ritual                         |

#### Objects: `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`

### Server-Initiated Commands (AI)

| Command/Response        | Description                              |
|------------------------|------------------------------------------|
| ok                     | Command succeeded                        |
| ko                     | Command failed                           |
| message \<dir>, \<msg>    | Received a broadcast message             |
| dead                   | Player has died                          |
| Elevation underway     | Elevation ritual started                  |
| Current level: \<lvl>   | Level after successful incantation        |
| ...                    | Various state updates (see [subject doc](B-YEP-400_zappy.pdf))   |

---

## GUI ↔️ Server Protocol

### Authentication

1. **GUI sends:** `GRAPHIC`
2. **Server responds:** Full world state via a sequence of protocol messages.

### Command Structure

- GUI commands are single-line, case-sensitive strings.
- The GUI can request information or send control commands to the server.

#### Example

```msg
msz\n      # Request map size
bct x y\n  # Request contents of the tile at (x,y)
mct\n      # Request contents of all tiles
tna\n      # Request team names
ppo #id\n  # Request position of player #id
```

### GUI-Initiated Commands

| Command          | Description                                 |
|------------------|---------------------------------------------|
| msz              | Get map size                                |
| bct x y          | Get contents of tile (x,y)                  |
| mct              | Get contents of all map tiles               |
| tna              | Get all team names                          |
| ppo #id          | Get player position                         |
| plv #id          | Get player level                            |
| pin #id          | Get player inventory                        |
| sgc              | Get current game time unit                  |
| sst n            | Set frequency (admin only, rarely used)     |

### Server-Initiated Commands (GUI)

The server pushes real-time updates to the GUI, including but not limited to:

| Command                | Description                                 |
|------------------------|---------------------------------------------|
| msz X Y                | Map size                                    |
| bct X Y q0...q6        | Tile contents (quantities for each resource)|
| tna \<team>             | Team name                                   |
| ppo #id X Y O          | Player position and orientation             |
| plv #id L              | Player level                                |
| pin #id X Y q0...q6    | Player inventory                            |
| pdi #id                | Player death                                |
| enw #egg #id X Y       | New egg laid                                |
| ebo #egg               | Egg hatches                                 |
| edi #egg               | Egg dies                                    |
| sgt T                  | Time unit change                            |
| seg \<team>             | Game ends, team wins                        |
| smg \<msg>              | Message to GUI                              |
| ...                    | See [protocol PDF](B-YEP-400_zappy_GUI_protocol.pdf) for full list              |

---

## Examples

### AI Client Example Session

```msg
[Client] connects to server
[Server] WELCOME\n
[Client] team1\n
[Server] 3\n
[Server] 10 10\n
[Client] Forward\n
[Server] ok\n
[Client] Look\n
[Server] [{tile1},{tile2},...]\n
[Client] Take food\n
[Server] ok\n
```

### GUI Client Example Session

```msg
[Client] connects to server
[Server] WELCOME\n
[Client] GRAPHIC\n
[Server] msz 10 10\n
[Server] bct 0 0 1 0 0 0 0 0 0\n
...
[Server] tna team1\n
[Server] tna team2\n
...
```

---

## Additional Notes

- The full protocol, including command/response sequences and edge cases, is defined in the [official protocol PDF](B-YEP-400_zappy_GUI_protocol.pdf).
- All components should robustly handle unexpected or out-of-order messages.
- For best results, implement a line buffer on both sending and receiving sides.
- The protocol is **stateless** with respect to the actual content of commands; the server tracks all game state and determines the validity of actions.
- **Timeouts:** Recommended to disconnect clients that are inactive for a pre-defined period.

---

## References

- [Project subject specification](B-YEP-400_zappy.pdf)
- [GUI protocol details](B-YEP-400_zappy_GUI_protocol.pdf)
- [README.md](../README.md)
