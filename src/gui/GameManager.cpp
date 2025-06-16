/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Manager implementation
*/

#include "GameManager.hpp"
#include <iostream>

/**
 * @brief Construct a new GameManager object.
 */
GameManager::GameManager() : _scene(nullptr), _mapWidth(0), _mapHeight(0)
{
}

/**
 * @brief Destroy the GameManager object and free resources.
 */
GameManager::~GameManager()
{
    for (auto& player : _players) {
        delete player.second;
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
 * @param scene Pointer to the scene object.
 */
void GameManager::initialize(Scenne* scene)
{
    _scene = scene;
}

/**
 * @brief Update the game state.
 */
void GameManager::update()
{
    if (!_scene) {
        std::cerr << "Scene not initialized" << std::endl;
        return;
    }
    _time++;
}

/**
 * @brief Set the map size and create the grid.
 *
 * @param width Map width.
 * @param height Map height.
 */
void GameManager::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
    _tiles.resize(height);
    for (auto& row : _tiles) {
        row.resize(width, nullptr);
    }
    createGrid();
}

/**
 * @brief Create the grid of tiles for the map.
 */
void GameManager::createGrid()
{
    for (int y = 0; y < _mapHeight; y++) {
        for (int x = 0; x < _mapWidth; x++) {
            createTile(x, y);
        }
    }
}

/**
 * @brief Create a player and add to the map.
 *
 * @param id Player ID.
 * @param teamName Team name.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param orientation Player orientation.
 */
void GameManager::createEgg(int id, int parentId, int x, int y)
{
    if (_eggs.find(id) != _eggs.end()) {
        std::cerr << "Egg " << id << " already exists" << std::endl;
        return;
    }

    Egg* egg = new Egg(id, parentId);
    egg->attachToScene(_scene->getSceneManager());
    egg->setScale(0.01f, 0.01f, 0.01f);
    _eggs[id] = egg;

    if (y >= 0 && y < _mapHeight && x >= 0 && x < _mapWidth && _tiles[y][x]) {
        _tiles[y][x]->addEgg(egg);
    } else {
        std::cerr << "Invalid tile for egg " << id << " at (" << x << ", " << y << ")" << std::endl;
    }
}

/**
 * @brief Create a tile at the specified coordinates.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 */
void GameManager::createTile(int x, int y)
{
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight) {
        std::cerr << "Tile coordinates out of bounds: (" << x << ", " << y
                  << ")" << std::endl;
        return;
    }

    Tile* tile = new Tile(x, y);

    _tiles[y][x] = tile;
    tile->attachToScene(_scene->getSceneManager());
    const float tileSize = 10.0f;
    float posX = static_cast<float>(x - (_mapWidth - 1) / 2.0f) * tileSize;
    float posZ = static_cast<float>(y - (_mapHeight - 1) / 2.0f) * tileSize;
    tile->setPosition(posX, 0.0f, posZ);
    tile->setScale(0.1f, 0.1f, 0.1f);

    if ((x + y) % 2 == 0) {
        tile->setColor({0.0f, 0.8f, 0.0f, 1.0f});
    } else {
        tile->setColor({0.03f, 0.1f, 0.03f, 1.0f});
    }
}

/**
 * @brief Create a player and add to the map.
 *
 * @param id Player ID.
 * @param teamName Team name.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param orientation Player orientation.
 */
void GameManager::createPlayer(int id, const std::string& teamName, int x,
                               int y, Orientation orientation)
{
    if (_players.find(id) != _players.end()) {
        std::cerr << "Player " << id << " already exists" << std::endl;
        return;
    }

    Player* player = new Player(id, teamName);
    player->attachToScene(_scene->getSceneManager());
    player->setOrientation(orientation);
    player->setScale(0.1f, 0.1f, 0.1f);
    _players[id] = player;

    if (y >= 0 && y < _mapHeight && x >= 0 && x < _mapWidth && _tiles[y][x]) {
        _tiles[y][x]->addPlayer(player);
    } else {
        std::cerr << "Invalid tile for player " << id << " at (" << x << ", " << y << ")" << std::endl;
    }
}

/**
 * @brief Update a player's position and orientation.
 *
 * @param id Player ID.
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param orientation Player orientation.
 */
void GameManager::updatePlayerPosition(int id, int x, int y,
                                       Orientation orientation)
{
    auto it = _players.find(id);
    if (it == _players.end()) {
        std::cerr << "Player " << id << " not found" << std::endl;
        return;
    }

    Player* player = it->second;
    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile) tile->removePlayer(player);
        }
    }
    if (y >= 0 && y < _mapHeight && x >= 0 && x < _mapWidth && _tiles[y][x]) {
        _tiles[y][x]->addPlayer(player);
    } else {
        std::cerr << "Invalid tile for player " << id << " at (" << x << ", " << y << ")" << std::endl;
    }
    player->setOrientation(orientation);
}

/**
 * @brief Update the resources on a tile.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param resources Map of resource names to quantities.
 */
void GameManager::updateTileContent(int x, int y,
                                    const std::map<std::string, int>& resources)
{
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight) {
        std::cerr << "Tile coordinates out of bounds: (" << x << ", " << y
                  << ")" << std::endl;
        return;
    }

    Tile* tile = _tiles[y][x];
    if (!tile) {
        std::cerr << "Tile not created at (" << x << ", " << y << ")"
                  << std::endl;
        return;
    }

    for (const auto& resource : resources) {
        tile->setResource(resource.first, resource.second);
    }
}

/**
 * @brief Get the map size.
 *
 * @return std::pair<int, int> The (width, height) of the map.
 */
std::pair<int, int> GameManager::getMapSize() const
{
    return std::make_pair(_mapWidth, _mapHeight);
}
