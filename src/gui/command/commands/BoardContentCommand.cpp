/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BoardContentCommand
*/

#include "BoardContentCommand.hpp"
#include <iostream>
#include <string>
#include <sstream>

BoardContentCommand::BoardContentCommand()
{
}

void BoardContentCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::istringstream iss(args);
    int x, y, quantity;
    iss >> x >> y;
    int resourceType = 0;

    while (iss >> quantity) {
        gameManager.createResource(static_cast<ResourceType>(resourceType), x, y, quantity);
        resourceType++;
    }
}
