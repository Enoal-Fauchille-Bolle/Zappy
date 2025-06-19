#ifndef SERVER_MESSAGE_COMMAND_HPP
#define SERVER_MESSAGE_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class ServerMessageCommand : public ICommand {
public:
    ServerMessageCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // SERVER_MESSAGE_COMMAND_HPP
