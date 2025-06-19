/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInventoryCommand
*/

#include "PlayerInventoryCommand.hpp"
#include <iostream>

PlayerInventoryCommand::PlayerInventoryCommand()
{
}

void PlayerInventoryCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerInventoryCommand " << args << std::endl;
}
