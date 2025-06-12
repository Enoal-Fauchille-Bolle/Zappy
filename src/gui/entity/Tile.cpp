/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile entity implementation
*/

#include "Tile.hpp"

Tile::Tile(int x, int y)
    : Entity("tile_" + std::to_string(x) + "_" + std::to_string(y), "cube.mesh"),
      _x(x), _y(y)
{
}

Tile::~Tile()
{
}

void Tile::setResource(const std::string &resourceName, int quantity)
{
    _resources[resourceName] = quantity;
    updateVisuals();
}

int Tile::getResource(const std::string &resourceName) const
{
    auto it = _resources.find(resourceName);
    if (it != _resources.end()) {
        return it->second;
    }
    return 0;
}

std::pair<int, int> Tile::getCoordinates() const
{
    return std::make_pair(_x, _y);
}

void Tile::updateVisuals()
{
}
