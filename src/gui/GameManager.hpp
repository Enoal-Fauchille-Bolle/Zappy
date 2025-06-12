/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game Manager class
*/

#ifndef GAME_MANAGER_HPP_
#define GAME_MANAGER_HPP_

#include "scenne/Scenne.hpp"
#include "entity/Player.hpp"
#include "entity/Tile.hpp"
#include <vector>
#include <map>

class GameManager {
    public:
        GameManager();
        ~GameManager();

        void initialize(Scenne* scene);
        void update();
        
        void setMapSize(int width, int height);
        void createTile(int x, int y);
        void createPlayer(int id, const std::string& teamName, int x, int y, Orientation orientation);
        void updatePlayerPosition(int id, int x, int y, Orientation orientation);
        void updateTileContent(int x, int y, const std::map<std::string, int>& resources);
        
        std::pair<int, int> getMapSize() const;

    private:
        Scenne* _scene;
        int _mapWidth;
        int _mapHeight;
        
        std::map<int, Player*> _players;
        std::vector<std::vector<Tile*>> _tiles;

        int _time = 0;
        
        void createGrid();
};

#endif /* !GAME_MANAGER_HPP_ */
