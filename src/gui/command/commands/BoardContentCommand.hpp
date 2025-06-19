#ifndef BOARD_CONTENT_COMMAND_HPP
#define BOARD_CONTENT_COMMAND_HPP

#include <string>
#include "../ICommand.hpp"
#include "../../GameManager.hpp"

class BoardContentCommand : public ICommand {
public:
    BoardContentCommand();
    void execute(const std::string& args, SimpleGameManager gameManager) override;
};

#endif // BOARD_CONTENT_COMMAND_HPP
