#ifndef SERVER_SET_TIME_COMMAND_HPP
#define SERVER_SET_TIME_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class ServerSetTimeCommand : public ICommand {
public:
    ServerSetTimeCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // SERVER_SET_TIME_COMMAND_HPP
