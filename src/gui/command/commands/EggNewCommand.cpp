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
    iss >> idStr >> parentIdStr >> x >> y;

    id = std::stoi(idStr.substr(1));
    parentId = std::stoi(parentIdStr.substr(1));
    gameManager.createEgg(id, parentId, x, y);
}
