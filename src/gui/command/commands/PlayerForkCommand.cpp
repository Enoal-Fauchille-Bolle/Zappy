/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerForkCommand
*/

#include "PlayerForkCommand.hpp"
#include <iostream>

PlayerForkCommand::PlayerForkCommand()
{
}

void PlayerForkCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerForkCommand " << args << std::endl;
}
