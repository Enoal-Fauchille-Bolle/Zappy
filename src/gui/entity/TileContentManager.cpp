/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile content manager implementation
*/

#include "TileContentManager.hpp"
#include "Player.hpp"
#include "Resources.hpp"
#include "Egg.hpp"
#include "Entity.hpp"
#include "../Types.hpp"
#include <algorithm>

/**
 * @brief Constructor for TileContentManager.
 * 
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 */
TileContentManager::TileContentManager(int x, int y) : _x(x), _y(y) {}

/**
 * @brief Destructor for TileContentManager.
 */
TileContentManager::~TileContentManager() {}

/**
 * @brief Add a player to this tile.
 * 
 * @param player Pointer to the player to add. Ignores duplicates.
 */
void TileContentManager::addPlayer(Player* player) {
    if (player && std::find(_players.begin(), _players.end(), player) == _players.end()) {
        _players.push_back(player);
    }
}

/**
 * @brief Remove a player from this tile.
 * 
 * @param player Pointer to the player to remove.
 */
void TileContentManager::removePlayer(Player* player) {
    auto it = std::remove(_players.begin(), _players.end(), player);
    if (it != _players.end()) {
        _players.erase(it, _players.end());
    }
}

/**
 * @brief Get all players on this tile.
 * 
 * @return Const reference to the vector of players.
 */
const std::vector<Player*>& TileContentManager::getPlayers() const {
    return _players;
}

/**
 * @brief Add a resource to this tile.
 * 
 * @param resource Pointer to the resource to add. Ignores duplicates.
 */
void TileContentManager::addResource(Resources* resource) {
    if (resource && std::find(_resources.begin(), _resources.end(), resource) == _resources.end()) {
        _resources.push_back(resource);
    }
}

/**
 * @brief Remove a resource from this tile.
 * 
 * @param resource Pointer to the resource to remove.
 */
void TileContentManager::removeResource(Resources* resource) {
    auto it = std::remove(_resources.begin(), _resources.end(), resource);
    if (it != _resources.end()) {
        _resources.erase(it, _resources.end());
    }
}

/**
 * @brief Get all resources on this tile.
 * 
 * @return Const reference to the vector of resources.
 */
const std::vector<Resources*>& TileContentManager::getResources() const {
    return _resources;
}

/**
 * @brief Add an egg to this tile.
 * 
 * @param egg Pointer to the egg to add. Ignores duplicates.
 */
void TileContentManager::addEgg(Egg* egg) {
    if (egg && std::find(_eggs.begin(), _eggs.end(), egg) == _eggs.end()) {
        _eggs.push_back(egg);
    }
}

/**
 * @brief Remove an egg from this tile.
 * 
 * @param egg Pointer to the egg to remove.
 */
void TileContentManager::removeEgg(Egg* egg) {
    auto it = std::remove(_eggs.begin(), _eggs.end(), egg);
    if (it != _eggs.end()) {
        _eggs.erase(it, _eggs.end());
    }
}

/**
 * @brief Get all eggs on this tile.
 * 
 * @return Const reference to the vector of eggs.
 */
const std::vector<Egg*>& TileContentManager::getEggs() const {
    return _eggs;
}

/**
 * @brief Add a generic entity to this tile.
 * 
 * @param entity Pointer to the entity to add. Ignores duplicates.
 */
void TileContentManager::addEntity(Entity* entity) {
    if (entity && std::find(_entities.begin(), _entities.end(), entity) == _entities.end()) {
        _entities.push_back(entity);
    }
}

/**
 * @brief Remove a generic entity from this tile.
 * 
 * @param entity Pointer to the entity to remove.
 */
void TileContentManager::removeEntity(Entity* entity) {
    auto it = std::remove(_entities.begin(), _entities.end(), entity);
    if (it != _entities.end()) {
        _entities.erase(it, _entities.end());
    }
}

/**
 * @brief Get all generic entities on this tile.
 * 
 * @return Const reference to the vector of entities.
 */
const std::vector<Entity*>& TileContentManager::getEntities() const {
    return _entities;
}

/**
 * @brief Set the quantity of a specific resource type for compatibility.
 * 
 * @param resourceName Name of the resource type.
 * @param quantity Quantity to set.
 */
void TileContentManager::setResourceQuantity(const std::string& resourceName, int quantity) {
    _resourceQuantities[resourceName] = quantity;
}

/**
 * @brief Get the quantity of a specific resource type.
 * 
 * @param resourceName Name of the resource type.
 * @return The quantity of the resource, or 0 if not found.
 */
int TileContentManager::getResourceQuantity(const std::string& resourceName) const {
    auto it = _resourceQuantities.find(resourceName);
    return (it != _resourceQuantities.end()) ? it->second : 0;
}

/**
 * @brief Get the world position of this tile.
 * 
 * @return Position struct containing the world coordinates.
 */
Position TileContentManager::getPosition() const {
    return Utils::calculateTilePosition(_x, _y, 10, 10);
}

/**
 * @brief Get the grid coordinates of this tile.
 * 
 * @return Pair containing the x and y coordinates.
 */
std::pair<int, int> TileContentManager::getCoordinates() const {
    return std::make_pair(_x, _y);
}

/**
 * @brief Check if this tile is empty of all entities.
 * 
 * @return True if no players, resources, eggs, or entities are on this tile.
 */
bool TileContentManager::isEmpty() const {
    return _players.empty() && _resources.empty() && _eggs.empty() && _entities.empty();
}
