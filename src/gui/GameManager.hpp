/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Simplified Game Manager class
*/

#ifndef SIMPLE_GAME_MANAGER_HPP_
#define SIMPLE_GAME_MANAGER_HPP_

#include "scenne/Scenne.hpp"
#include "entity/Player.hpp"
#include "entity/Egg.hpp"
#include "entity/TileDisplay.hpp"
#include "entity/Resources.hpp"
#include "Types.hpp"
#include <vector>
#include <map>

class CommandHandler;

class SimpleGameManager {
public:
    SimpleGameManager();
    ~SimpleGameManager();

    void initialize(Scenne* scene);
    void setMapSize(int width, int height);

    void createTile(int x, int y);

    void createPlayer(int id, const std::string& tesetResourcesamName, int x, int y, Orientation orientation);
    void updatePlayerPosition(int id, int x, int y, Orientation orientation);
    void removePlayer(int id);

    void createEgg(int id, int parentId, int x, int y);
    void eggToPlayer(int eggId);
    void removeEgg(int id);

    void createResource(ResourceType type, int x, int y, int quantity = 1);
    void removeResource(ResourceType type, int x, int y, int quantity = 1);

    void update();
    void readResponse(const std::string& response);
    std::pair<int, int> getMapSize() const;

    void cleanup();

private:
    Scenne* _scene;
    CommandHandler* _commandHandler;
    int _mapWidth, _mapHeight;

    PlayerMap _players;
    EggMap _eggs;
    std::vector<std::vector<TileDisplay*>> _tiles;

    void createGrid();
    TileDisplay* getTile(int x, int y);
    bool isValidPosition(int x, int y) const;
    void positionPlayerOnTile(Player* player, int x, int y);
    void positionResourceOnTile(Resources* resource, int x, int y, int index);
};

#endif /* !SIMPLE_GAME_MANAGER_HPP_ */
