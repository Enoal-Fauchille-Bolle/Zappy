/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** NetworkWorker
*/

#ifndef NETWORKWORKER_HPP_
#define NETWORKWORKER_HPP_

#include <memory>
#include <thread>
#include "message/MessageBuffer.hpp"
#include "message/MessageProtocol.hpp"
#include "POSIX_wrapper/connection/ConnectionManager.hpp"
#include "POSIX_wrapper/SocketTransport.hpp"

class NetworkWorker {
private:
    std::shared_ptr<ConnectionManager> connection_manager_;
    std::shared_ptr<SocketTransport> socket_transport_;
    std::shared_ptr<MessageProtocol> message_protocol_;
    std::shared_ptr<MessageBuffer<std::string>> send_buffer_;

    std::atomic<bool> running_;
    std::thread worker_thread_;

    void worker_loop();

public:
    NetworkWorker(
        std::shared_ptr<ConnectionManager> connection_manager,
        std::shared_ptr<SocketTransport> socket_transport,
        std::shared_ptr<MessageProtocol> message_protocol,
        std::shared_ptr<MessageBuffer<std::string>> send_buffer
    );
    ~NetworkWorker();

    void start();
    void stop();
};

#endif /* !NETWORKWORKER_HPP_ */
