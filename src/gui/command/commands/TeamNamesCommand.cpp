/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TeamNamesCommand
*/

#include "TeamNamesCommand.hpp"
#include <iostream>

TeamNamesCommand::TeamNamesCommand()
{
}

void TeamNamesCommand::execute(const std::string& args, SimpleGameManager& gameManager) {
    std::string teamName;
    std::istringstream iss(args);
    iss >> teamName;

    gameManager.addTeam(teamName);
}
