/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Manager implementation
*/

#include "GameManager.hpp"
#include <iostream>

GameManager::GameManager()
    :_scene(nullptr), _mapWidth(0), _mapHeight(0)
{
}

GameManager::~GameManager()
{
    // Clean up players
    for (auto& player : _players) {
        delete player.second;
    }

    // Clean up tiles
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
    // This method would be called from the main loop
    // It would process network updates and update the scene accordingly
}

void GameManager::setMapSize(int width, int height)
{
    _mapWidth = width;
    _mapHeight = height;
    
    // Initialize tile vector
    _tiles.resize(height);
    for (auto& row : _tiles) {
        row.resize(width, nullptr);
    }
    
    // Create visual grid
    createGrid();
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
        std::cerr << "Tile coordinates out of bounds: (" << x << ", " << y << ")" << std::endl;
        return;
    }
    
    // Create tile entity
    Tile* tile = new Tile(x, y);
    
    // Position it in the world
    float posX = static_cast<float>(x) * 10.0f - (_mapWidth * 5.0f);
    float posZ = static_cast<float>(y) * 10.0f - (_mapHeight * 5.0f);
    tile->setPosition(posX, 0.0f, posZ);
    
    // Store and attach to scene
    _tiles[y][x] = tile;
    tile->attachToScene(_scene->getSceneManager());
}

void GameManager::createPlayer(int id, const std::string& teamName, int x, int y, Orientation orientation)
{
    // Check if player already exists
    if (_players.find(id) != _players.end()) {
        std::cerr << "Player " << id << " already exists" << std::endl;
        return;
    }
    
    // Create new player
    Player* player = new Player(id, teamName);
    player->setOrientation(orientation);
    
    // Position it on the right tile
    float posX = static_cast<float>(x) * 10.0f - (_mapWidth * 5.0f);
    float posZ = static_cast<float>(y) * 10.0f - (_mapHeight * 5.0f);
    player->setPosition(posX, 2.0f, posZ); // Slightly above the tile
    
    // Store and attach to scene
    _players[id] = player;
    player->attachToScene(_scene->getSceneManager());
}

void GameManager::updatePlayerPosition(int id, int x, int y, Orientation orientation)
{
    auto it = _players.find(id);
    if (it == _players.end()) {
        std::cerr << "Player " << id << " not found" << std::endl;
        return;
    }
    
    // Update position
    Player* player = it->second;
    float posX = static_cast<float>(x) * 10.0f - (_mapWidth * 5.0f);
    float posZ = static_cast<float>(y) * 10.0f - (_mapHeight * 5.0f);
    player->setPosition(posX, 2.0f, posZ);
    player->setOrientation(orientation);
}

void GameManager::updateTileContent(int x, int y, const std::map<std::string, int>& resources)
{
    if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight) {
        std::cerr << "Tile coordinates out of bounds: (" << x << ", " << y << ")" << std::endl;
        return;
    }
    
    Tile* tile = _tiles[y][x];
    if (!tile) {
        std::cerr << "Tile not created at (" << x << ", " << y << ")" << std::endl;
        return;
    }
    
    // Update resources
    for (const auto& resource : resources) {
        tile->setResource(resource.first, resource.second);
    }
}

std::pair<int, int> GameManager::getMapSize() const
{
    return std::make_pair(_mapWidth, _mapHeight);
}
