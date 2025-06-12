/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Manager implementation
*/

#include "GameManager.hpp"
#include <iostream>

GameManager::GameManager() : _scene(nullptr), _mapWidth(0), _mapHeight(0)
{
}

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

void GameManager::initialize(Scenne* scene)
{
    _scene = scene;
}

void GameManager::update()
{
    if (!_scene) {
        std::cerr << "Scene not initialized" << std::endl;
        return;
    }

    _time++;
    for (auto& player : _players) {
        player.second->setPosition(player.first + ((player.first * 10.0f - (_mapHeight * 5.0f)) * _time / 100.0f), 10.0f, player.first * 10.0f - (_mapHeight * 5.0f));
    }
}

void GameManager::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
    _tiles.resize(height);
    for (auto& row : _tiles) {
        row.resize(width, nullptr);
    }
    createGrid();
    createPlayer(1, "TeamA", 0, 0, Orientation::NORTH);
    createPlayer(2, "TeamB", 1, 1, Orientation::EAST);
    createPlayer(3, "TeamC", 2, 2, Orientation::SOUTH);
    createPlayer(4, "TeamD", 3, 3, Orientation::WEST);
}

void GameManager::createGrid()
{
    for (int y = 0; y < _mapHeight; y++) {
        for (int x = 0; x < _mapWidth; x++) {
            createTile(x, y);
        }
    }
}

void GameManager::createTile(int x, int y)
{
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight) {
        std::cerr << "Tile coordinates out of bounds: (" << x << ", " << y
                  << ")" << std::endl;
        return;
    }

    Tile* tile = new Tile(x, y);

    // First store the tile, then attach to scene, then set color
    _tiles[y][x] = tile;
    tile->attachToScene(_scene->getSceneManager());
    float posX = static_cast<float>(x) * 15.0f - (_mapWidth * 5.0f);
    float posZ = static_cast<float>(y) * 15.0f - (_mapHeight * 5.0f);
    tile->setPosition(posX, 0.0f, posZ);
    tile->setScale(0.1f, 0.1f, 0.1f);

    // Set contrasting colors AFTER attaching to scene
    if ((x + y) % 2 == 0) {
        tile->setColor({1.0f, 0.0f, 1.0f, 0.0f});
    } else {
        tile->setColor({0.0f, 0.0f, 1.0f, 1.0f});  // Pure blue
    }

    // Debug output for each tile creation
    std::cout << "Creating tile at (" << x << ", " << y << ") with position ("
              << posX << ", 0.0, " << posZ << ")" << std::endl;
}

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
    float posX = static_cast<float>(x) * 10.0f - (_mapWidth * 5.0f);
    float posZ = static_cast<float>(y) * 10.0f - (_mapHeight * 5.0f);
    player->setPosition(posX, 10.0f, posZ);
    player->setScale(0.1f, 0.1f, 0.1f);
    _players[id] = player;
}

void GameManager::updatePlayerPosition(int id, int x, int y,
                                       Orientation orientation)
{
    auto it = _players.find(id);
    if (it == _players.end()) {
        std::cerr << "Player " << id << " not found" << std::endl;
        return;
    }

    Player* player = it->second;
    float posX = static_cast<float>(x) * 10.0f - (_mapWidth * 5.0f);
    float posZ = static_cast<float>(y) * 10.0f - (_mapHeight * 5.0f);
    player->setPosition(posX, 2.0f, posZ);
    player->setOrientation(orientation);
}

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

std::pair<int, int> GameManager::getMapSize() const
{
    return std::make_pair(_mapWidth, _mapHeight);
}
