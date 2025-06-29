/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggBornCommand
*/

#include "EggBornCommand.hpp"
#include <iostream>
#include <sstream>

EggBornCommand::EggBornCommand()
{
}

void EggBornCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::string idStr;
    int id;
    std::istringstream iss(args);

    if (!(iss >> idStr)) {
        std::cerr << "Error: Invalid arguments for EggBornCommand: " << args << std::endl;
        return;
    }
    try {
        std::string cleanIdStr = (!idStr.empty() && idStr.front() == '#') ? idStr.substr(1) : idStr;
        id = std::stoi(cleanIdStr);
        if (id < 0) {
            std::cerr << "Error: Invalid egg ID (must be positive): " << id << std::endl;
            return;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid egg ID in EggBornCommand: " << idStr << std::endl;
        return;
    }
    gameManager.removeEgg(id);
}
