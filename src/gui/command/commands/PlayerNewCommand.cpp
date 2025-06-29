/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerNewCommand
*/

#include "PlayerNewCommand.hpp"
#include "../../Types.hpp"
#include <iostream>
#include <sstream>

PlayerNewCommand::PlayerNewCommand()
{
}

void PlayerNewCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::string idStr;
    int id, x, y, lvl, orientation;
    std::string teamName;
    std::istringstream iss(args);

    if (!(iss >> idStr >> x >> y >> orientation >> lvl >> teamName)) {
        std::cerr << "Error: Invalid arguments for PlayerNewCommand: " << args << std::endl;
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
        std::cerr << "Error: Invalid player ID in PlayerNewCommand: " << idStr << std::endl;
        return;
    }
    gameManager.createPlayer(id, teamName, x, y, static_cast<Orientation>(orientation));
}
