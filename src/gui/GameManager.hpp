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

class SimpleGameManager {
public:
    SimpleGameManager();
    ~SimpleGameManager();

    // Core setup
    void initialize(Scenne* scene);
    void setMapSize(int width, int height);

    // Tile management
    void createTile(int x, int y);

    // Player management
    void createPlayer(int id, const std::string& teamName, int x, int y, Orientation orientation);
    void updatePlayerPosition(int id, int x, int y, Orientation orientation);
    void removePlayer(int id);

    // Egg management
    void createEgg(int id, int parentId, int x, int y);
    void removeEgg(int id);

    // Resource management
    void createResource(ResourceType type, int x, int y, int quantity = 1);
    void removeResource(ResourceType type, int x, int y, int quantity = 1);

    // Utility
    void update();
    std::pair<int, int> getMapSize() const;

private:
    Scenne* _scene;
    int _mapWidth, _mapHeight;
    
    PlayerMap _players;
    EggMap _eggs;
    std::vector<std::vector<TileDisplay*>> _tiles;
    
    // Helper methods
    void createGrid();
    TileDisplay* getTile(int x, int y);
    bool isValidPosition(int x, int y) const;
    void positionPlayerOnTile(Player* player, int x, int y);
    void positionResourceOnTile(Resources* resource, int x, int y, int index);
};

#endif /* !SIMPLE_GAME_MANAGER_HPP_ */
