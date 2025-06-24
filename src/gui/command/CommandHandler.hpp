/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CommandHandler
*/

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include "ICommand.hpp"
#include "../GameManager.hpp"

class CommandHandler {
public:
    CommandHandler();
    ~CommandHandler();

    void handleCommand(const std::string& command, const std::string& args, SimpleGameManager& gameManager);

private:
    std::unordered_map<std::string, std::unique_ptr<ICommand>> _commands;
};

#endif // COMMAND_HANDLER_HPP
