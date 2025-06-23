/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity implementation
*/

#include "Player.hpp"
#include "../Types.hpp"

std::map<int, std::string> Player::playerMeshes = {
    {1, "ogrehead.mesh"},
    {2, "robot.mesh"},
    {3, "ogrehead.mesh"},
    {4, "ogrehead.mesh"},
    {5, "ogrehead.mesh"},
    {6, "ogrehead.mesh"},
    {7, "ogrehead.mesh"},
    {8, "ogrehead.mesh"},
};

/**
 * @brief Construct a new Player object.
 *
 * @param id The player ID.
 * @param teamName The name of the team.
 */
Player::Player(int id, const std::string &teamName)
    : Entity("player_" + std::to_string(id), "ogrehead.mesh"),
      _playerId(id),
      _teamName(teamName),
      _level(1),
      _orientation(Orientation::NORTH)
{
}

/**
 * @brief Destroy the Player object.
 */
Player::~Player()
{
    this->getSceneNode()->getCreator()->destroyEntity(this->getOgreEntity());
}

/**
 * @brief Set the level of the player and update scale.
 *
 * @param level The new level.
 */
void Player::setLevel(int level)
{
    _level = level;
    float scale = 1.0f + (_level * 0.1f);
    setScale(scale, scale, scale);
}

/**
 * @brief Set the orientation of the player and update visuals.
 *
 * @param orientation The new orientation.
 */
void Player::setOrientation(Orientation orientation)
{
    _orientation = orientation;
    updateVisualOrientation();
}

/**
 * @brief Update the visual orientation of the player.
 */
void Player::updateVisualOrientation()
{
    switch (_orientation) {
        case Orientation::NORTH:
            setRotation(0, 180, 0);
            break;
        case Orientation::EAST:
            setRotation(0, 90, 0);
            break;
        case Orientation::SOUTH:
            setRotation(0, 0, 0);
            break;
        case Orientation::WEST:
            setRotation(0, 270, 0);
            break;
    }
}

/**
 * @brief Set the quantity of an inventory item.
 *
 * @param item The item name.
 * @param quantity The quantity to set.
 */
void Player::setInventoryItem(const std::string &item, int quantity)
{
    _inventory[item] = quantity;
}

/**
 * @brief Get the player's level.
 *
 * @return int The level.
 */
int Player::getLevel() const
{
    return _level;
}

/**
 * @brief Get the player's orientation.
 *
 * @return Orientation The orientation.
 */
Orientation Player::getOrientation() const
{
    return _orientation;
}

/**
 * @brief Get the quantity of an inventory item.
 *
 * @param item The item name.
 * @return int The quantity.
 */
int Player::getInventoryItem(const std::string &item) const
{
    auto it = _inventory.find(item);
    if (it != _inventory.end()) {
        return it->second;
    }
    return 0;
}

/**
 * @brief Get the player's team name.
 *
 * @return std::string The team name.
 */
std::string Player::getTeamName() const
{
    return _teamName;
}

/**
 * @brief Initialize the player entity.
 *
 * This function is called after the player entity is attached to a node.
 */
void Player::initialize()
{
}

/**
 * @brief Evolve the player to the next level.
 */
void Player::evolve()
{
    if (_level < Constants::MAX_PLAYER_LEVEL) {
        _level++;
        this->setScale(Constants::PLAYER_SCALE, Constants::PLAYER_SCALE, Constants::PLAYER_SCALE);
        replaceMesh(this->playerMeshes[_level]);
    }
}
