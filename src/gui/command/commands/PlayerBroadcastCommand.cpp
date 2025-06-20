/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerBroadcastCommand
*/

#include "PlayerBroadcastCommand.hpp"
#include <iostream>

PlayerBroadcastCommand::PlayerBroadcastCommand()
{
}

void PlayerBroadcastCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "PlayerBroadcastCommand " << args << std::endl;
}
