/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggBornCommand
*/

#include "EggBornCommand.hpp"
#include <iostream>

EggBornCommand::EggBornCommand()
{
}

void EggBornCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "EggBornCommand " << args << std::endl;
}
