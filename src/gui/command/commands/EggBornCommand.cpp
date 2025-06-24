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
    iss >> idStr;

    id = std::stoi(idStr.substr(1));
    gameManager.removeEgg(id);
}
