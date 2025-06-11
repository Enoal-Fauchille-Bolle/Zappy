/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity implementation
*/

#include "Player.hpp"

Player::Player(int id, const std::string &teamName)
    : Entity("player_" + std::to_string(id), "cube.mesh"),
      _playerId(id),
      _teamName(teamName),
      _level(1),
      _orientation(Orientation::NORTH)
{
}

Player::~Player()
{
}

void Player::setLevel(int level)
{
    _level = level;
    float scale = 1.0f + (_level * 0.1f);
    setScale(scale, scale, scale);
}

void Player::setOrientation(Orientation orientation)
{
    _orientation = orientation;
    updateVisualOrientation();
}

void Player::updateVisualOrientation()
{
    switch (_orientation) {
        case Orientation::NORTH:
            setRotation(0, 0, 0);
            break;
        case Orientation::EAST:
            setRotation(0, 90, 0);
            break;
        case Orientation::SOUTH:
            setRotation(0, 180, 0);
            break;
        case Orientation::WEST:
            setRotation(0, 270, 0);
            break;
    }
}

void Player::setInventoryItem(const std::string &item, int quantity)
{
    _inventory[item] = quantity;
}

int Player::getLevel() const
{
    return _level;
}

Orientation Player::getOrientation() const
{
    return _orientation;
}

int Player::getInventoryItem(const std::string &item) const
{
    auto it = _inventory.find(item);
    if (it != _inventory.end()) {
        return it->second;
    }
    return 0;
}

std::string Player::getTeamName() const
{
    return _teamName;
}
