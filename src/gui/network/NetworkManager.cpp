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

NetworkManager::~NetworkManager()
{
    network_worker_->stop();
    reconnection_service_->stop();
}

std::unique_ptr<NetworkManager>
    NetworkManager::create_instance(const std::string &host, int port)
{
    return std::unique_ptr<NetworkManager>(new NetworkManager(host, port));
}

bool NetworkManager::send_impl(const std::string &command)
{
    return send_buffer_->push(command);
}

std::string NetworkManager::receive_impl(bool blocking) {
    std::string received = "";

    if (blocking) {
        return receive_buffer_->pop();
    } else {
        receive_buffer_->tryPop(received);
    }
    return received;
}

bool NetworkManager::is_connected() const {
    return connection_manager_->isConnected();
}

NetworkManager* NetworkManager::get_instance() {
    std::lock_guard<std::mutex> lock(instance_mutex_);
    return instance_.get();
}

void NetworkManager::initialize(const std::string& host, int port) {
    std::lock_guard<std::mutex> lock(instance_mutex_);
    instance_.reset(new NetworkManager(host, port));;
}

bool NetworkManager::send(const std::string& command) {
    NetworkManager *mgr = get_instance();
    if (!mgr) {
        std::cerr << "NetworkManager not initialized. "
            "Call initialize() first." << std::endl;
        return false;
    }
    return mgr->send_impl(command);
}

std::string NetworkManager::receive(bool blocking) {
    NetworkManager *mgr = get_instance();
    if (!mgr) {
        std::cerr << "NetworkManager not initialized. "
            "Call initialize() first." << std::endl;
        return "";
    }
    return mgr->receive_impl(blocking);
}

bool NetworkManager::isConnected() {
    NetworkManager *mgr = get_instance();
    if (!mgr) {
        return false;
    }
    return mgr->is_connected();
}
