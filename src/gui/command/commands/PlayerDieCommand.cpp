/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerDieCommand
*/

#include "PlayerDieCommand.hpp"
#include <iostream>
#include <sstream>

PlayerDieCommand::PlayerDieCommand()
{
}

void PlayerDieCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::string idStr;
    int id;
    std::istringstream iss(args);

    if (!(iss >> idStr)) {
        std::cerr << "Error: Invalid arguments for PlayerDieCommand: " << args << std::endl;
        return;
    }
    try {
        std::string cleanIdStr = (!idStr.empty() && idStr.front() == '#') ? idStr.substr(1) : idStr;
        id = std::stoi(cleanIdStr);
        if (id <= 0) {
            std::cerr << "Error: Invalid player ID (must be positive): " << id << std::endl;
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid player ID in PlayerDieCommand: " << idStr << std::endl;
        return;
    }
    gameManager.removePlayer(id);
}
