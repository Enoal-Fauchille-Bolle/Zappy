/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerPositionCommand
*/

#include "PlayerPositionCommand.hpp"
#include <iostream>
#include <sstream>

PlayerPositionCommand::PlayerPositionCommand()
{
}

void PlayerPositionCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::string idStr;
    int id, x, y, orientation;

    std::istringstream iss(args);
    iss >> idStr >> x >> y >> orientation;
    id = std::stoi(idStr.substr(1));

    gameManager.updatePlayerPosition(id, x, y, static_cast<Orientation>(orientation));
}
