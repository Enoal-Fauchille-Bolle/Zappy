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

TileContentManager::TileContentManager(int x, int y) : _x(x), _y(y) {}

TileContentManager::~TileContentManager() {}

void TileContentManager::addPlayer(Player* player) {
    if (player && std::find(_players.begin(), _players.end(), player) == _players.end()) {
        _players.push_back(player);
    }
}

void TileContentManager::removePlayer(Player* player) {
    auto it = std::remove(_players.begin(), _players.end(), player);
    if (it != _players.end()) {
        _players.erase(it, _players.end());
    }
}

const std::vector<Player*>& TileContentManager::getPlayers() const {
    return _players;
}

// Resource management
void TileContentManager::addResource(Resources* resource) {
    if (resource && std::find(_resources.begin(), _resources.end(), resource) == _resources.end()) {
        _resources.push_back(resource);
    }
}

void TileContentManager::removeResource(Resources* resource) {
    auto it = std::remove(_resources.begin(), _resources.end(), resource);
    if (it != _resources.end()) {
        _resources.erase(it, _resources.end());
    }
}

const std::vector<Resources*>& TileContentManager::getResources() const {
    return _resources;
}

// Egg management
void TileContentManager::addEgg(Egg* egg) {
    if (egg && std::find(_eggs.begin(), _eggs.end(), egg) == _eggs.end()) {
        _eggs.push_back(egg);
    }
}

void TileContentManager::removeEgg(Egg* egg) {
    auto it = std::remove(_eggs.begin(), _eggs.end(), egg);
    if (it != _eggs.end()) {
        _eggs.erase(it, _eggs.end());
    }
}

const std::vector<Egg*>& TileContentManager::getEggs() const {
    return _eggs;
}

// Entity management
void TileContentManager::addEntity(Entity* entity) {
    if (entity && std::find(_entities.begin(), _entities.end(), entity) == _entities.end()) {
        _entities.push_back(entity);
    }
}

void TileContentManager::removeEntity(Entity* entity) {
    auto it = std::remove(_entities.begin(), _entities.end(), entity);
    if (it != _entities.end()) {
        _entities.erase(it, _entities.end());
    }
}

const std::vector<Entity*>& TileContentManager::getEntities() const {
    return _entities;
}

// Resource quantities (for compatibility with existing code)
void TileContentManager::setResourceQuantity(const std::string& resourceName, int quantity) {
    _resourceQuantities[resourceName] = quantity;
}

int TileContentManager::getResourceQuantity(const std::string& resourceName) const {
    auto it = _resourceQuantities.find(resourceName);
    return (it != _resourceQuantities.end()) ? it->second : 0;
}

// Utility
Position TileContentManager::getPosition() const {
    return Utils::calculateTilePosition(_x, _y, 10, 10);
}

std::pair<int, int> TileContentManager::getCoordinates() const {
    return std::make_pair(_x, _y);
}

bool TileContentManager::isEmpty() const {
    return _players.empty() && _resources.empty() && _eggs.empty() && _entities.empty();
}
