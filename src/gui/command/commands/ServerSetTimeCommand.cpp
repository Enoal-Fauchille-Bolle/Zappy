/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ServerSetTimeCommand
*/

#include "ServerSetTimeCommand.hpp"
#include <iostream>

ServerSetTimeCommand::ServerSetTimeCommand()
{
}

void ServerSetTimeCommand::execute(const std::string& args, SimpleGameManager& gameManager)
{
    int rate;
    std::istringstream iss(args);

    if (!(iss >> rate)) {
        std::cerr << "Error: Invalid arguments for ServerSetTimeCommand: " << args << std::endl;
        return;
    }
    if (rate <= 0) {
        std::cerr << "Error: Invalid tick rate (must be positive): " << rate << std::endl;
        return;
    }

    gameManager.setTickRate(rate);
}
