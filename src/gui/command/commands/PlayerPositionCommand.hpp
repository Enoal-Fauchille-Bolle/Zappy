/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerPositionCommand
*/

#ifndef PLAYER_POSITION_COMMAND_HPP
#define PLAYER_POSITION_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerPositionCommand : public ICommand {
public:
    PlayerPositionCommand();
    void execute(const std::string& args, SimpleGameManager& gameManager) override;
};

#endif // PLAYER_POSITION_COMMAND_HPP
