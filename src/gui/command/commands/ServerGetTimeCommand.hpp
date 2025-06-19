#ifndef SERVER_GET_TIME_COMMAND_HPP
#define SERVER_GET_TIME_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class ServerGetTimeCommand : public ICommand {
public:
    ServerGetTimeCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // SERVER_GET_TIME_COMMAND_HPP