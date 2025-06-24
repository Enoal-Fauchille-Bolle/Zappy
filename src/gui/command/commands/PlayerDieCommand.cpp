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
    iss >> idStr;

    id = std::stoi(idStr.substr(1));
    gameManager.removePlayer(id);
}
