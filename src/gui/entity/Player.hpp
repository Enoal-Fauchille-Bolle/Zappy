/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity class
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Entity.hpp"
#include "../Types.hpp"
#include <string>
#include <map>

class Player : public Entity {
    public:
        Player(int id, const std::string &teamName);
        ~Player();

        void initialize() override;

        void setLevel(int level);
        void setOrientation(Orientation orientation, bool animate = true);
        void setInventoryItem(const std::string &item, int quantity);

        int getLevel() const;
        Orientation getOrientation() const;
        int getInventoryItem(const std::string &item) const;
        std::string getTeamName() const;

        void evolve();

        // Animated movement API
        void slideTo(const Position& target);
        void update(float deltaTime);
        bool isMoving() const { return _isMoving; }
        void setPosition(const Position& pos); // Override to sync _currentPos

        // Rotation animation API
        void rotateTo(float targetYaw);
        bool isRotating() const { return _isRotating; }

        // void setMoveAndRotateSpeed(float moveSpeed, float rotateSpeed);

    private:
        int _playerId;
        std::string _teamName;
        int _level;
        Orientation _orientation;
        std::map<std::string, int> _inventory;
        // Animation fields
        Position _currentPos;
        Position _targetPos;
        bool _isMoving = false;
        float _moveSpeed = 8.0f; // Tiles per second (adjust as needed)
        // Rotation animation fields
        float _currentYaw = 0.0f;
        float _targetYaw = 0.0f;
        bool _isRotating = false;
        float _rotateSpeed = 360.0f; // Degrees per second

        static std::map<int, std::string> playerMeshes;

        void updateVisualOrientation();
};

#endif /* !PLAYER_HPP_ */
