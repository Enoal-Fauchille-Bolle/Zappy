/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** MessageProtocol
*/

#include "MessageProtocol.hpp"
#include <memory>
#include <string>

MessageProtocol::MessageProtocol(std::shared_ptr<MessageBuffer<std::string>> receive_buffer)
    : receive_buffer_(receive_buffer) {
}

void MessageProtocol::process_received_data(const std::string& data) {
    partial_message_ += data;

    std::string::size_type pos = 0;
    while ((pos = partial_message_.find('\n')) != std::string::npos) {
        std::string command = partial_message_.substr(0, pos);
        partial_message_.erase(0, pos + 1);

        receive_buffer_->push(command);
    }
}

std::string MessageProtocol::format_outgoing_message(const std::string& command) {
    return command + "\n";
}
