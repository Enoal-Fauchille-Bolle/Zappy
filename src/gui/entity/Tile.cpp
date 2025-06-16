/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile entity implementation
*/

#include "Tile.hpp"
#include "Player.hpp"
#include <algorithm>

/**
 * @brief Construct a new Tile object.
 *
 * @param x The X coordinate of the tile.
 * @param y The Y coordinate of the tile.
 */
Tile::Tile(int x, int y)
    : Entity("tile_" + std::to_string(x) + "_" + std::to_string(y), "cube.mesh"),
      _x(x), _y(y)
{
}

/**
 * @brief Destroy the Tile object.
 */
Tile::~Tile()
{
}

/**
 * @brief Set the quantity of a resource on the tile and update visuals.
 *
 * @param resourceName The name of the resource.
 * @param quantity The quantity to set.
 */
void Tile::setResource(const std::string &resourceName, int quantity)
{
    _resources[resourceName] = quantity;
    updateVisuals();
}

/**
 * @brief Get the quantity of a resource on the tile.
 *
 * @param resourceName The name of the resource.
 * @return int The quantity.
 */
int Tile::getResource(const std::string &resourceName) const
{
    auto it = _resources.find(resourceName);
    if (it != _resources.end()) {
        return it->second;
    }
    return 0;
}

/**
 * @brief Get the coordinates of the tile.
 *
 * @return std::pair<int, int> The (x, y) coordinates.
 */
std::pair<int, int> Tile::getCoordinates() const
{
    return std::make_pair(_x, _y);
}

/**
 * @brief Update the visual representation of the tile.
 */
void Tile::updateVisuals()
{
}

/**
 * @brief Add a player to the tile and set their position.
 *
 * @param player The player to add.
 */
void Tile::addPlayer(Player* player)
{
    if (std::find(_players.begin(), _players.end(), player) == _players.end()) {
        _players.push_back(player);
        float tileSize = 10.0f;
        int mapWidth = 10;  // Default fallback, should be set by GameManager
        int mapHeight = 10; // Default fallback, should be set by GameManager
        #ifdef TILE_MAP_SIZE_GLOBAL
        #endif
        float posX = static_cast<float>(_x - (mapWidth - 1) / 2.0f) * tileSize;
        float posZ = static_cast<float>(_y - (mapHeight - 1) / 2.0f) * tileSize;
        player->setPosition(posX, 5.0f, posZ);
    }
}

/**
 * @brief Remove a player from the tile.
 *
 * @param player The player to remove.
 */
void Tile::removePlayer(Player* player)
{
    auto it = std::remove(_players.begin(), _players.end(), player);
    if (it != _players.end())
        _players.erase(it, _players.end());
}

/**
 * @brief Get the list of players on the tile.
 *
 * @return const std::vector<Player*>& The vector of player pointers.
 */
const std::vector<Player*>& Tile::getPlayers() const
{
    return _players;
}

/**
 * @brief Add an entity to the tile.
 *
 * @param entity The entity to add.
 */
void Tile::addEntity(Entity* entity)
{
    if (std::find(_entities.begin(), _entities.end(), entity) == _entities.end())
        _entities.push_back(entity);
}

/**
 * @brief Remove an entity from the tile.
 *
 * @param entity The entity to remove.
 */
void Tile::removeEntity(Entity* entity)
{
    auto it = std::remove(_entities.begin(), _entities.end(), entity);
    if (it != _entities.end())
        _entities.erase(it, _entities.end());
}

/**
 * @brief Get the list of entities on the tile.
 *
 * @return const std::vector<Entity*>& The vector of entity pointers.
 */
const std::vector<Entity*>& Tile::getEntities() const
{
    return _entities;
}
