/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile entity class
*/

#ifndef TILE_HPP_
#define TILE_HPP_

#include "Entity.hpp"
#include <map>
#include <string>
#include <vector>

class Player; // Forward declaration
class Egg; // Forward declaration
class Resources; // Forward declaration

class Tile : public Entity {
    public:
        Tile(int x, int y);
        ~Tile();

        void setResource(const std::string &resourceName, int quantity);
        int getResource(const std::string &resourceName) const;

        std::pair<int, int> getCoordinates() const;
        void updateVisuals();

        void addPlayer(Player* player, int mapWidth, int mapHeight);
        void removePlayer(Player* player);
        const std::vector<Player*>& getPlayers() const;

        void addEgg(Egg* egg, int mapWidth, int mapHeight);
        void removeEgg(Egg* egg);
        const std::vector<Egg*>& getEggs() const;

        void addEntity(Entity* entity);
        void removeEntity(Entity* entity);
        const std::vector<Entity*>& getEntities() const;

        void addResource(Resources* resource);
        void removeResource(Resources* resource);
        const std::vector<Resources*>& getResources() const;

    private:
        int _x;
        int _y;
        std::map<std::string, int> _resources;
        std::vector<Player*> _players;
        std::vector<Egg*> _eggs;
        std::vector<Entity*> _entities;
        std::vector<Resources*> _resourcesEntities;
};

#endif /* !TILE_HPP_ */
