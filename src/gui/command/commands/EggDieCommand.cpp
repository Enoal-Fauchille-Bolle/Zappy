/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggDieCommand
*/

#include "EggDieCommand.hpp"
#include <iostream>
#include <sstream>

EggDieCommand::EggDieCommand()
{
}

void EggDieCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::cout << "EggDieCommand " << args << std::endl;
    std::string idStr;
    int id;

    std::istringstream iss(args);
    iss >> idStr;

    id = std::stoi(idStr.substr(1));
    gameManager.removeEgg(id);
}
