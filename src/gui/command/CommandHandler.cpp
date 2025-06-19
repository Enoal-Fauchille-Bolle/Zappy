#include <iostream>
#include "CommandHandler.hpp"
#include "commands/MapSizeCommand.hpp"
#include "commands/BoardContentCommand.hpp"
#include "commands/TeamNamesCommand.hpp"
#include "commands/PlayerNewCommand.hpp"
#include "commands/PlayerPositionCommand.hpp"
#include "commands/PlayerLevelCommand.hpp"
#include "commands/PlayerInventoryCommand.hpp"
#include "commands/PlayerExpelledCommand.hpp"
#include "commands/PlayerBroadcastCommand.hpp"
#include "commands/PlayerIncantationCommand.hpp"
#include "commands/PlayerIncantationEndCommand.hpp"
#include "commands/PlayerForkCommand.hpp"
#include "commands/PlayerDropCommand.hpp"
#include "commands/PlayerGetCommand.hpp"
#include "commands/PlayerDieCommand.hpp"
#include "commands/EggNewCommand.hpp"
#include "commands/EggBornCommand.hpp"
#include "commands/EggDieCommand.hpp"
#include "commands/ServerGetTimeCommand.hpp"
#include "commands/ServerSetTimeCommand.hpp"
#include "commands/ServerEndGameCommand.hpp"
#include "commands/ServerMessageCommand.hpp"
#include "commands/ServerUnknownCommand.hpp"
#include "commands/ServerBadParamCommand.hpp"

/**
 * @brief Constructor for CommandHandler.
 * 
 * Initializes the command handler with a reference to the game manager.
 */
CommandHandler::CommandHandler() {

    _commands["msz"] = std::make_unique<MapSizeCommand>();
    _commands["bct"] = std::make_unique<BoardContentCommand>();
    _commands["tna"] = std::make_unique<TeamNamesCommand>();
    _commands["pnw"] = std::make_unique<PlayerNewCommand>();
    _commands["ppo"] = std::make_unique<PlayerPositionCommand>();
    _commands["plv"] = std::make_unique<PlayerLevelCommand>();
    _commands["pin"] = std::make_unique<PlayerInventoryCommand>();
    _commands["pex"] = std::make_unique<PlayerExpelledCommand>();
    _commands["pbc"] = std::make_unique<PlayerBroadcastCommand>();
    _commands["pic"] = std::make_unique<PlayerIncantationCommand>();
    _commands["pie"] = std::make_unique<PlayerIncantationEndCommand>();
    _commands["pfk"] = std::make_unique<PlayerForkCommand>();
    _commands["pdr"] = std::make_unique<PlayerDropCommand>();
    _commands["pgt"] = std::make_unique<PlayerGetCommand>();
    _commands["pdi"] = std::make_unique<PlayerDieCommand>();
    _commands["enw"] = std::make_unique<EggNewCommand>();
    _commands["ebo"] = std::make_unique<EggBornCommand>();
    _commands["edi"] = std::make_unique<EggDieCommand>();
    _commands["sgt"] = std::make_unique<ServerGetTimeCommand>();
    _commands["sst"] = std::make_unique<ServerSetTimeCommand>();
    _commands["seg"] = std::make_unique<ServerEndGameCommand>();
    _commands["smg"] = std::make_unique<ServerMessageCommand>();
    _commands["suc"] = std::make_unique<ServerUnknownCommand>();
    _commands["sbp"] = std::make_unique<ServerBadParamCommand>();
}

/**
 * @brief Destructor for CommandHandler.
 * 
 * Cleans up the command handler resources.
 */
CommandHandler::~CommandHandler()
{
}

/**
 * @brief Handle a command with its arguments.
 * 
 * @param command The command string to handle.
 * @param args The arguments associated with the command.
 */
void CommandHandler::handleCommand(const std::string& command, const std::string& args, SimpleGameManager gameManager) {
    auto it = _commands.find(command);
    if (it != _commands.end()) {
        it->second->execute(args, gameManager);
    }
    return;
}
