/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** MessageProtocol
*/

/**
 * @file MessageProtocol.cpp
 * @brief Implementation of the MessageProtocol class for handling message formatting and parsing.
 */

#include "MessageProtocol.hpp"
#include <memory>
#include <string>

/**
 * @brief Construct a new MessageProtocol object.
 * @param receive_buffer Shared pointer to the receive buffer for incoming messages.
 */
MessageProtocol::MessageProtocol(std::shared_ptr<MessageBuffer<std::string>> receive_buffer)
    : receive_buffer_(receive_buffer) {
}

/**
 * @brief Process received data, extract complete messages, and push them to the receive buffer.
 * @param data The raw data received from the network.
 */
void MessageProtocol::process_received_data(const std::string& data) {
    partial_message_ += data;

    std::string::size_type pos = 0;
    while ((pos = partial_message_.find('\n')) != std::string::npos) {
        std::string command = partial_message_.substr(0, pos);
        partial_message_.erase(0, pos + 1);

        receive_buffer_->push(command);
    }
}

/**
 * @brief Format an outgoing command as a message to be sent over the network.
 * @param command The command string to format.
 * @return The formatted message string.
 */
std::string MessageProtocol::format_outgoing_message(const std::string& command) {
    return command + "\n";
}
