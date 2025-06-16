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

SimpleGameManager::SimpleGameManager() : _scene(nullptr), _mapWidth(0), _mapHeight(0) {}

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

void SimpleGameManager::initialize(Scenne* scene) {
    _scene = scene;
}

void SimpleGameManager::setMapSize(int width, int height) {
    _mapWidth = width;
    _mapHeight = height;
    _tiles.resize(height);
    for (auto& row : _tiles) {
        row.resize(width, nullptr);
    }
    createGrid();
}

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

void SimpleGameManager::update() {
    static int time = 0;
    time++;
    if (time == 100) {
        updatePlayerPosition(1, 1, 0, Orientation::NORTH);
    }
}

std::pair<int, int> SimpleGameManager::getMapSize() const {
    return std::make_pair(_mapWidth, _mapHeight);
}

void SimpleGameManager::createGrid() {
    for (int y = 0; y < _mapHeight; y++) {
        for (int x = 0; x < _mapWidth; x++) {
            createTile(x, y);
        }
    }
}

void SimpleGameManager::createTile(int x, int y) {
    TileDisplay* tile = new TileDisplay(x, y);
    tile->setMapSize(_mapWidth, _mapHeight);
    tile->attachToScene(_scene->getSceneManager());
    tile->initialize();
    Position pos = Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    tile->setPosition(pos);
    _tiles[y][x] = tile;
}

TileDisplay* SimpleGameManager::getTile(int x, int y) {
    if (!isValidPosition(x, y)) return nullptr;
    return _tiles[y][x];
}

bool SimpleGameManager::isValidPosition(int x, int y) const {
    return x >= 0 && x < _mapWidth && y >= 0 && y < _mapHeight;
}

void SimpleGameManager::positionPlayerOnTile(Player* player, int x, int y) {
    Position tilePos = Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    player->setPosition(tilePos.x, Constants::PLAYER_HEIGHT, tilePos.z);
}

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
