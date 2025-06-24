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
    iss >> idStr >> x >> y >> orientation >> lvl >> teamName;

    id = std::stoi(idStr.substr(1));
    gameManager.createPlayer(id, teamName, x, y, static_cast<Orientation>(orientation));
}
