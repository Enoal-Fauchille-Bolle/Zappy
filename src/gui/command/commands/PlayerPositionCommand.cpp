/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerPositionCommand
*/

#include "PlayerPositionCommand.hpp"
#include <iostream>

PlayerPositionCommand::PlayerPositionCommand()
{
}

void PlayerPositionCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerPositionCommand " << args << std::endl;
}
