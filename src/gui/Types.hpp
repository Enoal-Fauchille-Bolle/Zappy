/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Common types and macros
*/

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>

// Forward declarations
class Player;
class Resources;
class Egg;
class Entity;

// Type aliases for containers
using PlayerMap = std::map<int, Player*>;
using EggMap = std::map<int, Egg*>;

// Common structures
struct Position {
    float x, y, z;
    Position(float px = 0, float py = 0, float pz = 0) : x(px), y(py), z(pz) {}
};

struct Color {
    float r, g, b, a;
    Color(float cr = 1.0f, float cg = 1.0f, float cb = 1.0f, float ca = 1.0f) 
        : r(cr), g(cg), b(cb), a(ca) {}
};

// Enums
enum class Orientation {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};

enum class ResourceType {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME
};

// Constants
namespace Constants {
    constexpr float TILE_SIZE = 10.0f;
    constexpr float RESOURCE_HEIGHT = 8.0f;
    constexpr float PLAYER_HEIGHT = 6.0f;
    constexpr float EGG_HEIGHT = 6.0f;

    constexpr float RESOURCE_SCALE = 0.01f;
    constexpr float PLAYER_SCALE = 0.1f;
    constexpr float TILE_SCALE = 0.1f;
    constexpr float EGG_SCALE = 0.01f;

    constexpr int MAX_PLAYER_LEVEL = 8;
    constexpr int MAX_RESOURCES_PER_TILE = 16;
    constexpr float TILE_COVERAGE = 0.8f; // 80% of tile used for spreading
}

// Utility functions
namespace Utils {
    inline std::string generateResourceId(int x, int y, int index) {
        return "resource_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(index);
    }
    
    inline std::string generateTileId(int x, int y) {
        return "tile_" + std::to_string(x) + "_" + std::to_string(y);
    }
    
    inline Position calculateTilePosition(int x, int y, int mapWidth, int mapHeight) {
        float posX = static_cast<float>(x - (mapWidth - 1) / 2.0f) * Constants::TILE_SIZE;
        float posZ = static_cast<float>(y - (mapHeight - 1) / 2.0f) * Constants::TILE_SIZE;
        return Position(posX, 0.0f, posZ);
    }
    
    inline Position calculateResourceOffset(int index, float randomX = 0.0f, float randomZ = 0.0f) {
        const float halfSize = Constants::TILE_SIZE * Constants::TILE_COVERAGE * 0.5f;
        float offsetX = ((index % 4) - 1.5f) * (halfSize / 2.0f);
        float offsetZ = ((index / 4) - 1.5f) * (halfSize / 2.0f);
        return Position(offsetX + randomX, Constants::RESOURCE_HEIGHT, offsetZ + randomZ);
    }
}

#endif /* !TYPES_HPP_ */
