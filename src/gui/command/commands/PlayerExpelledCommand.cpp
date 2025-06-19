/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerExpelledCommand
*/

#include "PlayerExpelledCommand.hpp"
#include <iostream>

PlayerExpelledCommand::PlayerExpelledCommand()
{
}

void PlayerExpelledCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerExpelledCommand " << args << std::endl;
}
