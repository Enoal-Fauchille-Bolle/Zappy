# Zappy

## Table of Contents

- [Description](#description)
- [Features](#features)
  - [Core Gameplay](#core-gameplay)
  - [Technical Features](#technical-features)
- [Objectives](#objectives)
- [Understanding the Context](#understanding-the-context)
  - [Game Objective](#game-objective)
  - [Elevation Requirements](#elevation-requirements)
  - [World Mechanics](#world-mechanics)
- [Build Instructions](#build-instructions)
  - [How to Install OGRE Library](#how-to-install-ogre-library)
- [Usage](#usage)
  - [Server](#server)
  - [Graphical Client](#graphical-client)
  - [AI Client](#ai-client)
  - [Example Session](#example-session)
- [Contributors](#contributors)
- [Contributing](#contributing)
- [License](#license)

## Description

Zappy is a complex multiplayer network game inspired by Douglas Adams' "The Hitchhiker's Guide to the Galaxy". Players control Trantorian inhabitants on the spherical world of Trantor, where teams compete to elevate their members through mystical rituals involving resource collection and cooperation.

The project consists of three main components:

- **Server** (C): Manages the game world, resources, and player interactions
- **Graphical Client** (C++): Provides real-time visualization of the game world
- **AI Client** (Any language): Autonomous agents that control individual players

---

## Features

### Core Gameplay

- **Multiplayer Network Architecture**: TCP socket-based communication with support for multiple concurrent clients
- **Resource Management**: Six types of stones (linemate, deraumere, sibur, mendiane, phiras, thystame) plus food
- **Elevation System**: Players advance through 8 levels using specific resource combinations and group rituals
- **Spherical World**: Wrap-around map where players exit one side and enter the opposite
- **Vision System**: Level-based sight range that expands with elevation
- **Sound Broadcasting**: Directional audio messages between players
- **Player Reproduction**: Fork command creates eggs for team expansion

### Technical Features

- **Real-time Multiplexing**: Server uses `poll()` for efficient socket handling
- **Time-based Actions**: Configurable frequency system for action execution
- **Autonomous AI**: Self-directing clients with no human intervention required
- **Live Visualization**: GUI receives real-time updates from server
- **Resource Spawning**: Dynamic resource generation every 20 time units

---

## Objectives

This project aims to develop comprehensive skills in several key areas of computer science and software engineering:

### Technical Learning Goals

1. **Network Programming**: Master TCP socket communication, multiplexing with `poll()`, and real-time client-server architecture
2. **Concurrent Systems**: Handle multiple simultaneous connections without blocking, implementing efficient resource management
3. **Protocol Design**: Create robust communication protocols between different system components
4. **Cross-Language Integration**: Coordinate C server, C++ GUI, and multi-language AI clients
5. **Real-Time Systems**: Implement time-based action execution and event scheduling
6. **AI Development**: Design autonomous agents capable of strategic decision-making and cooperation

### Software Engineering Objectives

- **Modular Architecture**: Build three distinct but interconnected applications
- **Code Quality**: Write maintainable, efficient, and well-documented code
- **Testing & Debugging**: Develop skills in network debugging and multi-client testing
- **Performance Optimization**: Handle resource spawning, player actions, and GUI updates efficiently
- **Error Handling**: Implement robust error recovery for network failures and invalid commands

### Project Management Goals

- **Team Collaboration**: Coordinate development across multiple components
- **Documentation**: Create comprehensive technical documentation and user guides
- **Version Control**: Manage complex multi-binary project structure
- **Deployment**: Package and distribute networked applications

---

## Understanding the Context

Zappy simulates a complex ecosystem on the planet Trantor where teams compete to achieve spiritual elevation. The game mechanics create a rich environment for AI development and strategic thinking:

### Game Objective

The primary goal is for a team to have at least 6 players reach the maximum elevation level (level 8). This requires:

1. **Resource Collection**: Gathering the correct combination of stones for each elevation
2. **Team Coordination**: Multiple players of the same level must perform rituals together
3. **Survival Management**: Maintaining food supplies to prevent starvation (126 time units per food)
4. **Strategic Planning**: Balancing individual advancement with team cooperation

### Elevation Requirements

| Level | Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|---------|----------|-----------|-------|----------|--------|----------|
| 1→2   | 1       | 1        | 0         | 0     | 0        | 0      | 0        |
| 2→3   | 2       | 1        | 1         | 1     | 0        | 0      | 0        |
| 3→4   | 2       | 2        | 0         | 1     | 0        | 2      | 0        |
| 4→5   | 4       | 1        | 1         | 2     | 0        | 1      | 0        |
| 5→6   | 4       | 1        | 2         | 1     | 3        | 0      | 0        |
| 6→7   | 6       | 1        | 2         | 3     | 0        | 1      | 0        |
| 7→8   | 6       | 2        | 2         | 2     | 2        | 2      | 1        |

### World Mechanics

- **Trantorians** are peaceful, bodiless inhabitants who occupy entire tiles
- **Spherical World**: Players wrap around map edges (exit right, enter left)
- **Resources** are distributed according to density formulas based on map size
- **Time Management** is crucial with configurable action speeds
- **Communication** happens through directional broadcasts
- **Vision** is limited and expands with player level advancement
- **Reproduction** allows teams to create new players through the fork command

The game emphasizes artificial intelligence, network programming, and real-time systems design while providing an engaging competitive environment.

More details can be found in the [Subject](docs/B-YEP-400_zappy.pdf).

---

## Build Instructions

### Prerequisites

- GCC compiler for C code
- G++ compiler for C++
- Standard C and C++ libraries
- SFML library (recommended for GUI)
- Make utility

### How to Install OGRE Library

The graphical client uses the OGRE (Object-Oriented Graphics Rendering Engine) library. To install OGRE on Ubuntu/Debian-based systems:

```bash
sudo apt-get update
sudo apt-get install libogre-1.12-dev
```

For other distributions or more details, see the [OGRE official installation guide](https://ogrecave.github.io/ogre/api/latest/building-ogre.html).

don't forget to instal wayland.

### Compilation

```bash
# Clone the repository
git clone <repository-url>
cd zappy

# Build all components
make

# Individual builds
make zappy_server    # Build server
make zappy_gui      # Build GUI client
make zappy_ai       # Build AI client
```

### Makefile Targets

The Makefile should provide these targets:

- `zappy_server`: Compiles the game server
- `zappy_gui`: Compiles the graphical client
- `zappy_ai`: Compiles the AI client

---

## Usage

### Server

```bash
./zappy_server -p port -x width -y height -n team1 team2 ... -c clientsNb -f freq
```

**Options:**

- `-p port`: Port number for server
- `-x width`: World width
- `-y height`: World height
- `-n team1 team2 ...`: Team names
- `-c clientsNb`: Number of authorized clients per team
- `-f freq`: Reciprocal of time unit for action execution

### Graphical Client

```bash
./zappy_gui -p port -h machine
```

**Options:**

- `-p port`: Server port number
- `-h machine`: Server hostname

### AI Client

```bash
./zappy_ai -p port -n name -h machine
```

**Options:**

- `-p port`: Server port number
- `-n name`: Team name
- `-h machine`: Server hostname (localhost by default)

### Example Session

```bash
# Start server with 10x10 world, 2 teams, 4 clients per team
./zappy_server -p 4242 -x 10 -y 10 -n team1 team2 -c 4 -f 100

# Start GUI client
./zappy_gui -p 4242 -h localhost

# Start AI clients
./zappy_ai -p 4242 -n team1 -h localhost
./zappy_ai -p 4242 -n team2 -h localhost
```

---

## Contributors

- Evan MAHE: [GitHub/Mahe-Evan](https://github.com/Mahe-Evan)
- Renaud MANET: [GitHub/renman-ymd](https://github.com/renman-ymd)
- Jocelyn Jean-Elie: [GitHub/Jocelyn-JE](https://github.com/Jocelyn-JE)
- Kyllian CHANDELIER: [GitHub/KyllianChandelier](https://github.com/KyllianChandelier)
- Tom FELDKAMP: [GitHub/Azertoxe](https://github.com/Azertoxe)
- Enoal FAUCHILLE-BOLLE: [GitHub/Enoal-Fauchille-Bolle](https://github.com/Enoal-Fauchille-Bolle)

---

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## License

This project is released under the **GNU GPLv3 License**. See [LICENSE](LICENSE).
