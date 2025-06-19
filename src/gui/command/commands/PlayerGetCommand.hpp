#ifndef PLAYER_GET_COMMAND_HPP
#define PLAYER_GET_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerGetCommand : public ICommand {
public:
    PlayerGetCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_GET_COMMAND_HPP