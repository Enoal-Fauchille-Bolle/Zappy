/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerIncantationEndCommand
*/

#include "PlayerIncantationEndCommand.hpp"
#include <iostream>

PlayerIncantationEndCommand::PlayerIncantationEndCommand()
{
}

void PlayerIncantationEndCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerIncantationEndCommand " << args << std::endl;
}
