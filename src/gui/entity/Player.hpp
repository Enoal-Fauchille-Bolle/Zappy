/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity class
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Entity.hpp"
#include <string>
#include <map>

enum class Orientation {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
};

class Player : public Entity {
    public:
        Player(int id, const std::string &teamName);
        ~Player();

        void setLevel(int level);
        void setOrientation(Orientation orientation);
        void setInventoryItem(const std::string &item, int quantity);
        
        int getLevel() const;
        Orientation getOrientation() const;
        int getInventoryItem(const std::string &item) const;
        std::string getTeamName() const;

    private:
        int _playerId;
        std::string _teamName;
        int _level;
        Orientation _orientation;
        std::map<std::string, int> _inventory;

        void updateVisualOrientation();
};

#endif /* !PLAYER_HPP_ */
