/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile content manager - handles objects on tiles
*/

#ifndef TILE_CONTENT_MANAGER_HPP_
#define TILE_CONTENT_MANAGER_HPP_

#include "../Types.hpp"
#include <vector>

class Player;
class Resources;
class Egg;
class Entity;

class TileContentManager {
public:
    TileContentManager(int x, int y);
    ~TileContentManager();

    void addPlayer(Player* player);
    void removePlayer(Player* player);
    const std::vector<Player*>& getPlayers() const;

    void addResource(Resources* resource);
    void removeResource(Resources* resource);
    const std::vector<Resources*>& getResources() const;

    void addEgg(Egg* egg);
    void removeEgg(Egg* egg);
    const std::vector<Egg*>& getEggs() const;

    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    const std::vector<Entity*>& getEntities() const;

    void setResourceQuantity(const std::string& resourceName, int quantity);
    int getResourceQuantity(const std::string& resourceName) const;

    Position getPosition() const;
    std::pair<int, int> getCoordinates() const;
    bool isEmpty() const;

private:
    int _x, _y;
    std::vector<Player*> _players;
    std::vector<Resources*> _resources;
    std::vector<Egg*> _eggs;
    std::vector<Entity*> _entities;
    std::map<std::string, int> _resourceQuantities;
};

#endif /* !TILE_CONTENT_MANAGER_HPP_ */
