#ifndef SERVER_UNKNOWN_COMMAND_HPP
#define SERVER_UNKNOWN_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class ServerUnknownCommand : public ICommand {
public:
    ServerUnknownCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // SERVER_UNKNOWN_COMMAND_HPP
