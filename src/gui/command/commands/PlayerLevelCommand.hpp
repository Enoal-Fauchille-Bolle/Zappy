/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerLevelCommand
*/

#ifndef PLAYER_LEVEL_COMMAND_HPP
#define PLAYER_LEVEL_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerLevelCommand : public ICommand {
public:
    PlayerLevelCommand();
    void execute(const std::string& args, SimpleGameManager& gameManager) override;
};

#endif // PLAYER_LEVEL_COMMAND_HPP
