/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BoardContentCommand
*/

#include "BoardContentCommand.hpp"
#include <iostream>
#include <string>

BoardContentCommand::BoardContentCommand()
{
}

void BoardContentCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    (void)gameManager;
    std::cout << "BoardContentCommand " << args << std::endl;
}
