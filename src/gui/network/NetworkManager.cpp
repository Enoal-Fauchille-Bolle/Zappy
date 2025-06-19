/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** NetworkManager
*/

#include "NetworkManager.hpp"
#include <iostream>
#include <memory>

// Static member definitions
std::unique_ptr<NetworkManager> NetworkManager::instance_ = nullptr;
std::mutex NetworkManager::instance_mutex_;

/**
 * @brief Construct a new NetworkManager object.
 * @param host The server host address.
 * @param port The server port.
 */
NetworkManager::NetworkManager(const std::string& host, int port)
{
    connection_manager_ = std::make_shared<ConnectionManager>(host, port);
    socket_transport_ = std::make_shared<SocketTransport>(connection_manager_);

    send_buffer_ = std::make_shared<MessageBuffer<std::string>>();
    receive_buffer_ = std::make_shared<MessageBuffer<std::string>>();

    message_protocol_ = std::make_shared<MessageProtocol>(receive_buffer_);
    reconnection_service_ =
      std::make_shared<ReconnectionService>(connection_manager_);
    network_worker_ = std::make_shared<NetworkWorker>(
        connection_manager_, socket_transport_, message_protocol_, send_buffer_
    );

    reconnection_service_->start();
    network_worker_->start();
}

/**
 * @brief Destroy the NetworkManager object and stop network services.
 */
NetworkManager::~NetworkManager()
{
    network_worker_->stop();
    reconnection_service_->stop();
}

/**
 * @brief Create a new instance of NetworkManager.
 * @param host The server host address.
 * @param port The server port.
 * @return std::unique_ptr<NetworkManager> The created instance.
 */
std::unique_ptr<NetworkManager>
    NetworkManager::create_instance(const std::string &host, int port)
{
    return std::unique_ptr<NetworkManager>(new NetworkManager(host, port));
}

/**
 * @brief Send a command to the server (internal implementation).
 * @param command The command string to send.
 * @return true if the command was pushed to the buffer, false otherwise.
 */
bool NetworkManager::send_impl(const std::string &command)
{
    send_buffer_->push(command);
    return true;
}

/**
 * @brief Receive a message from the server (internal implementation).
 * @param blocking Whether to block until a message is received.
 * @return The received message string.
 */
std::string NetworkManager::receive_impl(bool blocking) {
    std::string received = "";

    if (blocking) {
        return receive_buffer_->pop();
    } else {
        receive_buffer_->tryPop(received);
    }
    return received;
}

/**
 * @brief Check if the connection to the server is active.
 * @return true if connected, false otherwise.
 */
bool NetworkManager::is_connected() const {
    return connection_manager_->isConnected();
}

/**
 * @brief Get the singleton instance of NetworkManager.
 * @return Pointer to the NetworkManager instance.
 */
NetworkManager* NetworkManager::get_instance() {
    std::lock_guard<std::mutex> lock(instance_mutex_);
    return instance_.get();
}

/**
 * @brief Initialize the NetworkManager singleton instance.
 * @param host The server host address.
 * @param port The server port.
 */
void NetworkManager::initialize(const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(instance_mutex_);
    instance_.reset(new NetworkManager(host, port));;
}

/**
 * @brief Send a command to the server using the singleton instance.
 * @param command The command string to send.
 * @return true if the command was sent, false otherwise.
 */
bool NetworkManager::send(const std::string& command) {
    NetworkManager *mgr = get_instance();
    if (!mgr) {
        std::cerr << "NetworkManager not initialized. "
            "Call initialize() first." << std::endl;
        return false;
    }
    return mgr->send_impl(command);
}

/**
 * @brief Receive a message from the server using the singleton instance.
 * @param blocking Whether to block until a message is received.
 * @return The received message string.
 */
std::string NetworkManager::receive(bool blocking) {
    NetworkManager *mgr = get_instance();
    if (!mgr) {
        std::cerr << "NetworkManager not initialized. "
            "Call initialize() first." << std::endl;
        return "";
    }
    return mgr->receive_impl(blocking);
}

/**
 * @brief Check if the singleton instance is connected to the server.
 * @return true if connected, false otherwise.
 */
bool NetworkManager::isConnected() {
    NetworkManager *mgr = get_instance();
    if (!mgr) {
        return false;
    }
    return mgr->is_connected();
}
