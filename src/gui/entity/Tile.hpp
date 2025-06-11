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

class Tile : public Entity {
    public:
        Tile(int x, int y);
        ~Tile();

        void setResource(const std::string &resourceName, int quantity);
        int getResource(const std::string &resourceName) const;
        
        std::pair<int, int> getCoordinates() const;
        void updateVisuals();

    private:
        int _x;
        int _y;
        std::map<std::string, int> _resources;
};

#endif /* !TILE_HPP_ */
