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

    if (!(iss >> idStr >> x >> y >> orientation)) {
        std::cerr << "Error: Invalid arguments for PlayerPositionCommand: " << args << std::endl;
        return;
    }
    try {
        std::string cleanIdStr = (!idStr.empty() && idStr.front() == '#') ? idStr.substr(1) : idStr;
        id = std::stoi(cleanIdStr);
        if (id < 0) {
            std::cerr << "Error: Invalid player ID (must be positive): " << id << std::endl;
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid player ID in PlayerPositionCommand: " << idStr << std::endl;
        return;
    }
    gameManager.updatePlayerPosition(id, x, y, static_cast<Orientation>(orientation));
}
