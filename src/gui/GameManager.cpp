/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Simplified Game Manager implementation
*/

#include "GameManager.hpp"
#include <iostream>
#include <cstdlib>
#include <random>

/**
 * @brief Default constructor for SimpleGameManager.
 * 
 * Initializes the game manager with null scene and zero map dimensions.
 */
SimpleGameManager::SimpleGameManager() : _scene(nullptr), _mapWidth(0), _mapHeight(0) {}

/**
 * @brief Destructor for SimpleGameManager.
 * 
 * Cleans up all allocated resources including players, eggs, and tiles.
 */
SimpleGameManager::~SimpleGameManager() {
    for (auto& [id, player] : _players) {
        delete player;
    }

    for (auto& [id, egg] : _eggs) {
        delete egg;
    }

    for (auto& row : _tiles) {
        for (auto& tile : row) {
            delete tile;
        }
    }
}

/**
 * @brief Initialize the game manager with a scene.
 * 
 * @param scene Pointer to the scene object that will be used for rendering.
 */
void SimpleGameManager::initialize(Scenne* scene) {
    _scene = scene;
}

/**
 * @brief Set the map size and initialize the tile grid.
 * 
 * @param width The width of the map in tiles.
 * @param height The height of the map in tiles.
 */
void SimpleGameManager::setMapSize(int width, int height) {
    _mapWidth = width;
    _mapHeight = height;
    _tiles.resize(height);
    for (auto& row : _tiles) {
        row.resize(width, nullptr);
    }
    createGrid();
}

/**
 * @brief Create a new player at the specified position.
 * 
 * @param id Unique identifier for the player.
 * @param teamName Name of the team the player belongs to.
 * @param x X coordinate on the map.
 * @param y Y coordinate on the map.
 * @param orientation Initial orientation of the player.
 */
void SimpleGameManager::createPlayer(int id, const std::string& teamName, int x, int y, Orientation orientation) {
    if (_players.find(id) != _players.end()) {
        std::cerr << "Player " << id << " already exists" << std::endl;
        return;
    }
    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid position for player " << id << ": (" << x << ", " << y << ")" << std::endl;
        return;
    }
    Player* player = new Player(id, teamName);
    player->attachToScene(_scene->getSceneManager());
    player->initialize();
    player->setOrientation(orientation);
    player->setScale(Constants::PLAYER_SCALE, Constants::PLAYER_SCALE, Constants::PLAYER_SCALE);
    _players[id] = player;
    positionPlayerOnTile(player, x, y);
    getTile(x, y)->getContentManager()->addPlayer(player);
}

/**
 * @brief Update the position and orientation of an existing player.
 * 
 * @param id Identifier of the player to update.
 * @param x New X coordinate on the map.
 * @param y New Y coordinate on the map.
 * @param orientation New orientation of the player.
 */
void SimpleGameManager::updatePlayerPosition(int id, int x, int y, Orientation orientation) {
    auto it = _players.find(id);
    if (it == _players.end()) {
        std::cerr << "Player " << id << " not found" << std::endl;
        return;
    }

    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid position for player " << id << ": (" << x << ", " << y << ")" << std::endl;
        return;
    }

    Player* player = it->second;

    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile) tile->getContentManager()->removePlayer(player);
        }
    }

    positionPlayerOnTile(player, x, y);
    getTile(x, y)->getContentManager()->addPlayer(player);
    player->setOrientation(orientation);
}

/**
 * @brief Remove a player from the game.
 * 
 * @param id Identifier of the player to remove.
 */
void SimpleGameManager::removePlayer(int id) {
    auto it = _players.find(id);
    if (it == _players.end()) return;

    Player* player = it->second;

    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile) tile->getContentManager()->removePlayer(player);
        }
    }

    delete player;
    _players.erase(it);
}

/**
 * @brief Create a new egg at the specified position.
 * 
 * @param id Unique identifier for the egg.
 * @param parentId Identifier of the parent player.
 * @param x X coordinate on the map.
 * @param y Y coordinate on the map.
 */
void SimpleGameManager::createEgg(int id, int parentId, int x, int y) {
    if (_eggs.find(id) != _eggs.end()) {
        std::cerr << "Egg " << id << " already exists" << std::endl;
        return;
    }

    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid position for egg " << id << ": (" << x << ", " << y << ")" << std::endl;
        return;
    }

    Egg* egg = new Egg(id, parentId);
    egg->attachToScene(_scene->getSceneManager());
    egg->initialize();
    egg->setScale(Constants::EGG_SCALE, Constants::EGG_SCALE, Constants::EGG_SCALE);

    Position tilePos = Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    egg->setPosition(tilePos.x, Constants::EGG_HEIGHT, tilePos.z);

    _eggs[id] = egg;
    getTile(x, y)->getContentManager()->addEgg(egg);
}

/**
 * @brief Remove an egg from the game.
 * 
 * @param id Identifier of the egg to remove.
 */
void SimpleGameManager::removeEgg(int id) {
    auto it = _eggs.find(id);
    if (it == _eggs.end()) return;

    Egg* egg = it->second;

    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile) tile->getContentManager()->removeEgg(egg);
        }
    }

    delete egg;
    _eggs.erase(it);
}

/**
 * @brief Create resources of the specified type at the given position.
 * 
 * @param type Type of resource to create.
 * @param x X coordinate on the map.
 * @param y Y coordinate on the map.
 * @param quantity Number of resources to create (default: 1).
 */
void SimpleGameManager::createResource(ResourceType type, int x, int y, int quantity) {
    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid position for resource: (" << x << ", " << y << ")" << std::endl;
        return;
    }

    TileDisplay* tile = getTile(x, y);
    if (!tile) {
        std::cerr << "No tile found at (" << x << ", " << y << ")" << std::endl;
        return;
    }

    int currentResourceCount = tile->getContentManager()->getResources().size();

    for (int i = 0; i < quantity; ++i) {
        int resourceIndex = currentResourceCount + i;
        Resources* resource = new Resources(Utils::generateResourceId(x, y, resourceIndex), "knot.mesh", type);
        resource->attachToScene(_scene->getSceneManager());
        resource->initialize();
        positionResourceOnTile(resource, x, y, resourceIndex);
        tile->getContentManager()->addResource(resource);
    }
}

/**
 * @brief Update the game state and handle time-based events.
 */
void SimpleGameManager::update() {
    static int time = 0;
    time++;
    if (time == 100) {
        updatePlayerPosition(1, 1, 0, Orientation::NORTH);
    }
}

/**
 * @brief Get the current map size.
 * 
 * @return A pair containing the width and height of the map.
 */
std::pair<int, int> SimpleGameManager::getMapSize() const {
    return std::make_pair(_mapWidth, _mapHeight);
}

/**
 * @brief Create the complete grid of tiles for the map.
 */
void SimpleGameManager::createGrid() {
    for (int y = 0; y < _mapHeight; y++) {
        for (int x = 0; x < _mapWidth; x++) {
            createTile(x, y);
        }
    }
}

/**
 * @brief Create a single tile at the specified coordinates.
 * 
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 */
void SimpleGameManager::createTile(int x, int y) {
    TileDisplay* tile = new TileDisplay(x, y);
    tile->setMapSize(_mapWidth, _mapHeight);
    tile->attachToScene(_scene->getSceneManager());
    tile->initialize();
    Position pos = Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    tile->setPosition(pos);
    _tiles[y][x] = tile;
}

/**
 * @brief Get a tile at the specified coordinates.
 * 
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 * @return Pointer to the tile, or nullptr if coordinates are invalid.
 */
TileDisplay* SimpleGameManager::getTile(int x, int y) {
    if (!isValidPosition(x, y)) return nullptr;
    return _tiles[y][x];
}

/**
 * @brief Check if the given coordinates are within the map bounds.
 * 
 * @param x X coordinate to check.
 * @param y Y coordinate to check.
 * @return True if the position is valid, false otherwise.
 */
bool SimpleGameManager::isValidPosition(int x, int y) const {
    return x >= 0 && x < _mapWidth && y >= 0 && y < _mapHeight;
}

/**
 * @brief Position a player on the specified tile.
 * 
 * @param player Pointer to the player to position.
 * @param x X coordinate of the target tile.
 * @param y Y coordinate of the target tile.
 */
void SimpleGameManager::positionPlayerOnTile(Player* player, int x, int y) {
    Position tilePos = Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    player->setPosition(tilePos.x, Constants::PLAYER_HEIGHT, tilePos.z);
}

/**
 * @brief Position a resource on the specified tile with randomized offset.
 * 
 * @param resource Pointer to the resource to position.
 * @param x X coordinate of the target tile.
 * @param y Y coordinate of the target tile.
 * @param index Index for calculating unique position offset.
 */
void SimpleGameManager::positionResourceOnTile(Resources* resource, int x, int y, int index) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-0.25f, 0.25f);
    Position tilePos = Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    float randomX = dist(gen);
    float randomZ = dist(gen);
    Position offset = Utils::calculateResourceOffset(index, randomX, randomZ);
    resource->setPosition(tilePos.x + offset.x, offset.y, tilePos.z + offset.z);
}
