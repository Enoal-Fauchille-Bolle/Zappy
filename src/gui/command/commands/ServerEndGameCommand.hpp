#ifndef SERVER_END_GAME_COMMAND_HPP
#define SERVER_END_GAME_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class ServerEndGameCommand : public ICommand {
public:
    ServerEndGameCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // SERVER_END_GAME_COMMAND_HPP