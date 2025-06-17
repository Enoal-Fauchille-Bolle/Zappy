/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** NetworkManager
*/

#ifndef NETWORKMANAGER_HPP_
#define NETWORKMANAGER_HPP_

#include <memory>
#include "message/MessageProtocol.hpp"
#include "POSIX_wrapper/SocketTransport.hpp"
#include "POSIX_wrapper/connection/ConnectionManager.hpp"
#include "POSIX_wrapper/connection/ReconnectionService.hpp"
#include "NetworkWorker.hpp"

class NetworkManager {
private:
    std::shared_ptr<ConnectionManager> connection_manager_;
    std::shared_ptr<SocketTransport> socket_transport_;
    std::shared_ptr<MessageProtocol> message_protocol_;
    std::shared_ptr<ReconnectionService> reconnection_service_;
    std::shared_ptr<NetworkWorker> network_worker_;

    std::shared_ptr<MessageBuffer<std::string>> send_buffer_;
    std::shared_ptr<MessageBuffer<std::string>> receive_buffer_;

    static std::unique_ptr<NetworkManager> instance_;
    static std::mutex instance_mutex_;

public:
    // Static interface
    static void initialize(const std::string &host, int port);
    static bool send(const std::string &command);
    static std::string receive(bool blocking = true);
    static bool isConnected();

    ~NetworkManager();
private:
    NetworkManager(const std::string &host, int port);

    static std::unique_ptr<NetworkManager>
        create_instance(const std::string &host, int port);
    bool send_impl(const std::string &command);
    std::string receive_impl(bool blocking = true);
    bool is_connected() const;

    static NetworkManager* get_instance();
};

#endif /* !NETWORKMANAGER_HPP_ */
