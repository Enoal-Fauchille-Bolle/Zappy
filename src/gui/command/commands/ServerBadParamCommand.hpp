/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ServerBadParamCommand
*/

#ifndef SERVER_BAD_PARAM_COMMAND_HPP
#define SERVER_BAD_PARAM_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class ServerBadParamCommand : public ICommand {
public:
    ServerBadParamCommand();
    void execute(const std::string& args, SimpleGameManager& gameManager) override;
};

#endif // SERVER_BAD_PARAM_COMMAND_HPP
