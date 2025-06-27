/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggNewCommand
*/

#include "EggNewCommand.hpp"
#include <iostream>
#include <sstream>

EggNewCommand::EggNewCommand()
{
}

void EggNewCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::string idStr, parentIdStr;
    int id, parentId, x, y;
    std::istringstream iss(args);

    if (!(iss >> idStr >> parentIdStr >> x >> y)) {
        std::cerr << "Error: Invalid arguments for EggNewCommand: " << args << std::endl;
        return;
    }
    try {
        std::string cleanIdStr = (!idStr.empty() && idStr.front() == '#') ? idStr.substr(1) : idStr;
        std::string cleanParentIdStr = (!parentIdStr.empty() && parentIdStr.front() == '#') ? parentIdStr.substr(1) : parentIdStr;
        id = std::stoi(cleanIdStr);
        parentId = std::stoi(cleanParentIdStr);
        if (id < 0) {
            std::cerr << "Error: Invalid egg ID (must be positive): " << id << std::endl;
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Failed to parse IDs in EggNewCommand: " << idStr << " or " << parentIdStr << std::endl;
        return;
    }
    gameManager.createEgg(id, parentId, x, y);
}
