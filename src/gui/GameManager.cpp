/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Simplified Game Manager implementation
*/

#include "GameManager.hpp"
#include "network/NetworkManager.hpp"
#include "command/CommandHandler.hpp"
#include <iostream>
#include <cstdlib>
#include <random>

/**
 * @brief Default constructor for SimpleGameManager.
 *
 * Initializes the game manager with null scene and zero map dimensions.
 */
SimpleGameManager::SimpleGameManager() :
        _scene(nullptr),
        _commandHandler(new CommandHandler()),
        _mapWidth(0),
        _mapHeight(0)
        // _tickRate(1.0f) // Default tick rate
{
}

/**
 * @brief Destructor for SimpleGameManager.
 *
 * Note: cleanup() should be called before this destructor to avoid segfaults
 */
SimpleGameManager::~SimpleGameManager()
{
    if (!_players.empty() || !_eggs.empty() || !_tiles.empty()) {
        std::cerr
            << "Warning: Resources not properly cleaned up before destructor"
            << std::endl;
    }
}

/**
 * @brief Initialize the game manager with a scene.
 *
 * @param scene Pointer to the scene object that will be used for rendering.
 */
void SimpleGameManager::initialize(Scene* scene)
{
    _scene = scene;
}

/**
 * @brief Set the map size and initialize the tile grid.
 *
 * @param width The width of the map in tiles.
 * @param height The height of the map in tiles.
 */
void SimpleGameManager::setMapSize(int width, int height)
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
 * @brief Create a new player at the specified position.
 *
 * @param id Unique identifier for the player.
 * @param teamName Name of the team the player belongs to.
 * @param x X coordinate on the map.
 * @param y Y coordinate on the map.
 * @param orientation Initial orientation of the player.
 */
void SimpleGameManager::createPlayer(int id, const std::string& teamName, int x,
                                     int y, Orientation orientation)
{
    if (_players.find(id) != _players.end()) {
        std::cerr << "Player " << id << " already exists" << std::endl;
        return;
    }
    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid position for player " << id << ": (" << x << ", "
                  << y << ")" << std::endl;
        return;
    }
    Player* player = new Player(id, teamName);
    player->attachToScene(_scene->getSceneManager());
    player->initialize();
    player->setScale(Constants::PLAYER_SCALE, Constants::PLAYER_SCALE,
                     Constants::PLAYER_SCALE);
    // Set initial position directly (no slide)
    Position tilePos = Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    player->setPosition(Position(tilePos.x, Constants::PLAYER_HEIGHT, tilePos.z));
    player->setOrientation(orientation, false);
    // player->setMoveAndRotateSpeed(2.0f * _tickRate, 90.0f * _tickRate);
    _players[id] = player;
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
void SimpleGameManager::updatePlayerPosition(int id, int x, int y,
                                             Orientation orientation)
{
    auto it = _players.find(id);
    if (it == _players.end()) {
        std::cerr << "Player " << id << " not found" << std::endl;
        return;
    }

    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid position for player " << id << ": (" << x << ", "
                  << y << ")" << std::endl;
        return;
    }

    Player* player = it->second;

    // Find previous position (current tile)
    int prevX = -1, prevY = -1;
    for (int ty = 0; ty < _mapHeight; ++ty) {
        for (int tx = 0; tx < _mapWidth; ++tx) {
            auto* tile = _tiles[ty][tx];
            if (tile) {
                const auto& players = tile->getContentManager()->getPlayers();
                if (std::find(players.begin(), players.end(), player) != players.end()) {
                    prevX = tx;
                    prevY = ty;
                    break;
                }
            }
        }
        if (prevX != -1) break;
    }

    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile)
                tile->getContentManager()->removePlayer(player);
        }
    }

    // Detect wrap
    bool animate = true;
    if (prevX != -1 && prevY != -1) {
        if ((prevX == 0 && x == _mapWidth - 1) || (prevX == _mapWidth - 1 && x == 0) ||
            (prevY == 0 && y == _mapHeight - 1) || (prevY == _mapHeight - 1 && y == 0)) {
            animate = false;
        }
    }

    positionPlayerOnTile(player, x, y, animate);
    getTile(x, y)->getContentManager()->addPlayer(player);
    player->setOrientation(orientation);
}

/**
 * @brief Remove a player from the game.
 *
 * @param id Identifier of the player to remove.
 */
void SimpleGameManager::removePlayer(int id)
{
    auto it = _players.find(id);
    if (it == _players.end())
        return;

    Player* player = it->second;

    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile)
                tile->getContentManager()->removePlayer(player);
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
void SimpleGameManager::createEgg(int id, int parentId, int x, int y)
{
    if (_eggs.find(id) != _eggs.end()) {
        std::cerr << "Egg " << id << " already exists" << std::endl;
        return;
    }

    if (!isValidPosition(x, y)) {
        std::cerr << "Invalid position for egg " << id << ": (" << x << ", "
                  << y << ")" << std::endl;
        return;
    }

    Egg* egg = new Egg(id, parentId);
    egg->attachToScene(_scene->getSceneManager());
    egg->initialize();
    egg->setScale(Constants::EGG_SCALE, Constants::EGG_SCALE,
                  Constants::EGG_SCALE);

    Position tilePos =
        Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    egg->setPosition(tilePos.x, Constants::EGG_HEIGHT, tilePos.z);

    _eggs[id] = egg;
    getTile(x, y)->getContentManager()->addEgg(egg);
}

/**
 * @brief Convert an egg to a player.
 *
 * @param eggId Identifier of the egg to convert.
 */
void SimpleGameManager::eggToPlayer(int eggId)
{
    auto it = _eggs.find(eggId);
    if (it == _eggs.end()) {
        std::cerr << "Egg " << eggId << " not found" << std::endl;
        return;
    }
}

/**
 * @brief Remove an egg from the game.
 *
 * @param id Identifier of the egg to remove.
 */
void SimpleGameManager::removeEgg(int id)
{
    auto it = _eggs.find(id);
    if (it == _eggs.end())
        return;

    Egg* egg = it->second;

    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile)
                tile->getContentManager()->removeEgg(egg);
        }
    }

    delete egg;
    _eggs.erase(it);
}

/** * @brief update resources of the specified type from the given position.
 *
 * @param type Type of resource to remove.
 * @param x X coordinate on the map.
 * @param y Y coordinate on the map.
 * @param quantity Number of resources have on tile.
 */

void SimpleGameManager::updateResource(ResourceType type, int x, int y, int quantity)
{
    TileDisplay* tile = getTile(x, y);
    if (!tile) {
        std::cerr << "No tile found at (" << x << ", " << y << ")" << std::endl;
        return;
    }

    int currentResourceCount = tile->getContentManager()->getResourceQuantity(type);

    if (currentResourceCount < quantity) {
        createResource(type, x, y, quantity - currentResourceCount);
    } else if (currentResourceCount > quantity) {
        removeResource(type, x, y, currentResourceCount - quantity);
    }
    tile->getContentManager()->setResourceQuantity(type, quantity);
}

/**
 * @brief Create resources of the specified type at the given position.
 *
 * @param type Type of resource to create.
 * @param x X coordinate on the map.
 * @param y Y coordinate on the map.
 * @param quantity Number of resources to create.
 */
void SimpleGameManager::createResource(ResourceType type, int x, int y, int quantity) {

    TileDisplay* tile = getTile(x, y);
    if (!tile) {
        std::cerr << "No tile found at (" << x << ", " << y << ")" << std::endl;
        return;
    }

    int index = tile->getContentManager()->getResources().size();

    for (int i = 0; i < quantity; ++i) {
        if (type == ResourceType::FOOD) {
            Resources* resource = new Resources(Utils::generateResourceId(type, x, y, index), "fish.mesh", type);
            resource->attachToScene(_scene->getSceneManager());
            resource->initialize();
            resource->setScale(0.5f, 0.5f, 0.5f);
            positionResourceOnTile(resource, x, y, index);
            tile->getContentManager()->addResource(resource);
        } else {
            Resources* resource = new Resources(Utils::generateResourceId(type, x, y, index), "knot.mesh", type);
            resource->attachToScene(_scene->getSceneManager());
            resource->initialize();
            positionResourceOnTile(resource, x, y, index);
            tile->getContentManager()->addResource(resource);
        }
        index++;
    }
}

/**
 * @brief Update the game state and handle time-based events.
 */
void SimpleGameManager::update()
{
    // Animate all players
    float deltaTime = 1.0f / 60.0f; // Assume 60 FPS for now
    for (auto& pair : _players) {
        if (pair.second)
            pair.second->update(deltaTime);
    }
    std::string response = NetworkManager::receive(false);
    readResponse(response);
}

/**
 * @brief Read and process the server response.
 *
 * @param response The response string from the server.
 */
void SimpleGameManager::readResponse(const std::string& response)
{
    if (response.length() < 3) {
        return;
    }
    std::string args{};
    std::string command = response.substr(0, 3);
    if (response.length() > 4)
        args = response.substr(4);
    _commandHandler->handleCommand(command, args, *this);
}

/*
 * @brief Get the current map size.
 *
 * @return A pair containing the width and height of the map.
 */
std::pair<int, int> SimpleGameManager::getMapSize() const
{
    return std::make_pair(_mapWidth, _mapHeight);
}

/**
 * @brief Create the complete grid of tiles for the map.
 */
void SimpleGameManager::createGrid()
{
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
void SimpleGameManager::createTile(int x, int y)
{
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
TileDisplay* SimpleGameManager::getTile(int x, int y)
{
    if (!isValidPosition(x, y))
        return nullptr;
    return _tiles[y][x];
}

/**
 * @brief Check if the given coordinates are within the map bounds.
 *
 * @param x X coordinate to check.
 * @param y Y coordinate to check.
 * @return True if the position is valid, false otherwise.
 */
bool SimpleGameManager::isValidPosition(int x, int y) const
{
    return x >= 0 && x < _mapWidth && y >= 0 && y < _mapHeight;
}

/**
 * @brief Position a player on the specified tile.
 *
 * @param player Pointer to the player to position.
 * @param x X coordinate of the target tile.
 * @param y Y coordinate of the target tile.
 */
void SimpleGameManager::positionPlayerOnTile(Player* player, int x, int y, bool animate)
{
    Position tilePos =
        Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    if (animate)
        player->slideTo(Position(tilePos.x, Constants::PLAYER_HEIGHT, tilePos.z));
    else
        player->setPosition(Position(tilePos.x, Constants::PLAYER_HEIGHT, tilePos.z));
}

/**
 * @brief Get the map of players currently in the game.
 *
 * @return A map containing player IDs and their corresponding Player objects.
 */
PlayerMap SimpleGameManager::getPlayers() const
{
    return _players;
}

/**
 * @brief Position a resource on the specified tile with randomized offset.
 *
 * @param resource Pointer to the resource to position.
 * @param x X coordinate of the target tile.
 * @param y Y coordinate of the target tile.
 * @param index Index for calculating unique position offset.
 */
void SimpleGameManager::positionResourceOnTile(Resources* resource, int x,
                                               int y, int index)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(-0.25f, 0.25f);
    Position tilePos =
        Utils::calculateTilePosition(x, y, _mapWidth, _mapHeight);
    float randomX = dist(gen);
    float randomZ = dist(gen);
    Position offset = Utils::calculateResourceOffset(index, randomX, randomZ);
    resource->setPosition(tilePos.x + offset.x, offset.y, tilePos.z + offset.z);
}

/**
 * @brief Remove a specified quantity of resources of a given type from a tile.
 *
 * @param type Type of resource to remove.
 * @param x X coordinate of the tile.
 * @param y Y coordinate of the tile.
 * @param quantity Number of resources to remove.
 */
void SimpleGameManager::removeResource(ResourceType type, int x, int y, int quantity)
{
    TileDisplay* tile = getTile(x, y);
    if (!tile) {
        std::cerr << "No tile found at (" << x << ", " << y << ")" << std::endl;
        return;
    }

    std::vector<Resources*> resourcesToRemove;
    const auto& resources = tile->getContentManager()->getResources();

    for (const auto& res : resources) {
        if (res->getResourceType() == type) {
            resourcesToRemove.push_back(res);
            if (resourcesToRemove.size() >= static_cast<size_t>(quantity)) {
                break;
            }
        }
    }

    int removedCount = 0;
    for (auto* resource : resourcesToRemove) {
        if (removedCount >= quantity) break;
        tile->getContentManager()->removeResource(resource);
        delete resource;
        removedCount++;
    }
}

/**
 * @brief Set the tick rate for the game manager.
 *
 * @param rate The new tick rate in milliseconds.
 */
void SimpleGameManager::setTickRate(int rate)
{
    _tickRate = rate;
}

/**
 * @brief Get the current tick rate of the game manager.
 *
 * @return The tick rate in milliseconds.
 */
int SimpleGameManager::getTickRate() const
{
    return _tickRate;
}


/**
 * @brief Add a team name to the game manager.
 *
 * @param teamName The name of the team to add.
 */
void SimpleGameManager::addTeam(const std::string& teamName)
{
    _teamNames.insert(teamName);
}

/**
 * @brief Get the set of team names currently registered in the game manager.
 *
 * @return A set containing the names of all teams.
 */
std::unordered_set<std::string> SimpleGameManager::getTeams() const
{
    return _teamNames;
}

/**
 * @brief Clean up all game resources before shutdown
 *
 * This method must be called before Ogre shutdown to prevent segfaults
 */
void SimpleGameManager::cleanup()
{
    for (auto& [id, player] : _players) {
        if (player) {
            for (auto& row : _tiles) {
                for (auto& tile : row) {
                    if (tile) {
                        tile->getContentManager()->removePlayer(player);
                    }
                }
            }
            delete player;
        }
    }
    _players.clear();
    for (auto& [id, egg] : _eggs) {
        if (egg) {
            for (auto& row : _tiles) {
                for (auto& tile : row) {
                    if (tile) {
                        tile->getContentManager()->removeEgg(egg);
                    }
                }
            }
            delete egg;
        }
    }
    _eggs.clear();
    for (auto& row : _tiles) {
        for (auto& tile : row) {
            if (tile) {
                const auto& resources =
                    tile->getContentManager()->getResources();
                for (auto* resource : resources) {
                    delete resource;
                }
                delete tile;
            }
        }
    }
    _tiles.clear();
    if (_commandHandler) {
        delete _commandHandler;
        _commandHandler = nullptr;
    }
}

/**
 * @brief Set the tick rate for the game manager.
 *
 * @param tickRate The desired tick rate in ticks per second.
 */
// void SimpleGameManager::setTickRate(float tickRate)
// {
//     _tickRate = tickRate;
//     // Example: base speeds (tweak as needed)
//     float baseMoveSpeed = 2.0f * tickRate;    // tiles per second
//     float baseRotateSpeed = 90.0f * tickRate; // degrees per second
//     for (auto& pair : _players) {
//         if (pair.second)
//             pair.second->setMoveAndRotateSpeed(baseMoveSpeed, baseRotateSpeed);
//     }
// }
