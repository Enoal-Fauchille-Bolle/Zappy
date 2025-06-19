#ifndef PLAYER_BROADCAST_COMMAND_HPP
#define PLAYER_BROADCAST_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class PlayerBroadcastCommand : public ICommand {
public:
    PlayerBroadcastCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // PLAYER_BROADCAST_COMMAND_HPP