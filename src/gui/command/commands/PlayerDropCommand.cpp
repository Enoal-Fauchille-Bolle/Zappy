/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerDropCommand
*/

#include "PlayerDropCommand.hpp"
#include <iostream>

PlayerDropCommand::PlayerDropCommand()
{
}

void PlayerDropCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerDropCommand " << args << std::endl;
}
