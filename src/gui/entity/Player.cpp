/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity implementation
*/

#include "Player.hpp"
#include "../Types.hpp"
#include <cmath>

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
      _orientation(Orientation::NORTH),
      _currentPos(0, Constants::PLAYER_HEIGHT, 0),
      _targetPos(0, Constants::PLAYER_HEIGHT, 0),
      _isMoving(false),
      _moveSpeed(90.0f),
      _currentYaw(0.0f),
      _targetYaw(0.0f),
      _isRotating(false),
      _rotateSpeed(1000.0f) // Degrees per second
{
}

/**
 * @brief Destroy the Player object.
 */
Player::~Player()
{
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
 * @brief Rotate the player to face a specific yaw angle.
 *
 * @param targetYaw The target yaw angle in degrees.
 */
void Player::rotateTo(float targetYaw)
{
    // Normalize angles to [0, 360)
    _targetYaw = fmodf(targetYaw + 360.0f, 360.0f);
    _isRotating = true;
}

/**
 * @brief Set the orientation of the player and update visuals.
 *
 * @param orientation The new orientation.
 */
void Player::setOrientation(Orientation orientation, bool animate)
{
    if (_orientation == orientation)
        return;
    _orientation = orientation;
    float newYaw = 0.0f;
    switch (_orientation) {
        case Orientation::NORTH: newYaw = 180.0f; break;
        case Orientation::EAST:  newYaw = 90.0f; break;
        case Orientation::SOUTH: newYaw = 0.0f; break;
        case Orientation::WEST:  newYaw = 270.0f; break;
    }
    if (animate) {
        rotateTo(newYaw);
    } else {
        _currentYaw = newYaw;
        _targetYaw = newYaw;
        _isRotating = false;
        setRotation(0, newYaw, 0);
    }
}

/**
 * @brief Update the player entity.
 *
 * @param deltaTime The time elapsed since the last update.
 */
void Player::update(float deltaTime)
{
    if (_isMoving) {
        float dx = _targetPos.x - _currentPos.x;
        float dy = _targetPos.y - _currentPos.y;
        float dz = _targetPos.z - _currentPos.z;
        float dist = std::sqrt(dx*dx + dy*dy + dz*dz);
        if (dist < 0.01f) {
            _currentPos = _targetPos;
            Entity::setPosition(_targetPos);
            _isMoving = false;
            return;
        }
        float step = _moveSpeed * deltaTime;
        float t = (dist < step) ? 1.0f : (step / dist);
        _currentPos.x += dx * t;
        _currentPos.y += dy * t;
        _currentPos.z += dz * t;
        Entity::setPosition(_currentPos);
    }

    // Animate rotation
    if (_isRotating) {
        float diff = _targetYaw - _currentYaw;
        // Shortest path
        if (diff > 180.0f) diff -= 360.0f;
        if (diff < -180.0f) diff += 360.0f;
        float step = _rotateSpeed * deltaTime;
        if (fabs(diff) < 1.0f) {
            _currentYaw = _targetYaw;
            _isRotating = false;
        } else {
            _currentYaw += (diff > 0 ? 1 : -1) * std::fmin(fabs(diff), step);
            _currentYaw = fmodf(_currentYaw + 360.0f, 360.0f);
        }
        setRotation(0, _currentYaw, 0);
    }
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

void Player::setPosition(const Position& pos)
{
    _currentPos = pos;
    Entity::setPosition(pos);
}

void Player::slideTo(const Position& target)
{
    _targetPos = target;
    _isMoving = true;
}

void Player::updateVisualOrientation()
{
    // No-op: handled by rotation animation
}

// void Player::setMoveAndRotateSpeed(float moveSpeed, float rotateSpeed)
// {
//     _moveSpeed = moveSpeed;
//     _rotateSpeed = rotateSpeed;
// }
