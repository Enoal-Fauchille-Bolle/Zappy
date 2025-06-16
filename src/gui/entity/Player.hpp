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

extern std::map<int, std::string> playerMeshes; // Meshes for different player IDs

class Player : public Entity {
    public:
        Player(int id, const std::string &teamName);
        ~Player();

        void initialize() override;

        void setLevel(int level);
        void setOrientation(Orientation orientation);
        void setInventoryItem(const std::string &item, int quantity);

        int getLevel() const;
        Orientation getOrientation() const;
        int getInventoryItem(const std::string &item) const;
        std::string getTeamName() const;

        void evolve();

    private:
        int _playerId;
        std::string _teamName;
        int _level;
        Orientation _orientation;
        std::map<std::string, int> _inventory;

        void updateVisualOrientation();
};

#endif /* !PLAYER_HPP_ */
