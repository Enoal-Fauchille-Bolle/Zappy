/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** SocketTransport
*/

#ifndef SOCKETTRANSPORT_HPP_
#define SOCKETTRANSPORT_HPP_

#include <cstddef>
#include <memory>
#include "connection/ConnectionManager.hpp"

class SocketTransport {
private:
    static constexpr size_t RECV_BUFFER_SIZE = 4096;
    std::shared_ptr<ConnectionManager> connection_manager_;

public:
    explicit SocketTransport(
        std::shared_ptr<ConnectionManager> connection_manager);

    bool send_raw(const std::string& data);
    std::string receive_raw();
};

#endif /* !SOCKETTRANSPORT_HPP_ */
