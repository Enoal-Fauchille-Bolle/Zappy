/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerGetCommand
*/

#include "PlayerGetCommand.hpp"
#include <iostream>

PlayerGetCommand::PlayerGetCommand()
{
}

void PlayerGetCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerGetCommand " << args << std::endl;
}
