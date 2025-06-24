/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** MessageProtocol
*/

#ifndef MESSAGEPROTOCOL_HPP_
#define MESSAGEPROTOCOL_HPP_

#include <memory>
#include <string>
#include "MessageBuffer.hpp"

class MessageProtocol {
    private:
        std::string partial_message_;
        std::shared_ptr<MessageBuffer<std::string>> receive_buffer_;

    public:
        explicit MessageProtocol(std::shared_ptr<MessageBuffer<std::string>> receive_buffer);

        void process_received_data(const std::string& data);
        static std::string format_outgoing_message(const std::string& command);
};

#endif /* !MESSAGEPROTOCOL_HPP_ */
