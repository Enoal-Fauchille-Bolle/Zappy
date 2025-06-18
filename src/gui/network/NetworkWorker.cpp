/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** NetworkWorker
*/

#include "NetworkWorker.hpp"

/**
 * @brief Construct a new NetworkWorker object.
 * @param connection_manager Shared pointer to the ConnectionManager.
 * @param socket_transport Shared pointer to the SocketTransport.
 * @param message_protocol Shared pointer to the MessageProtocol.
 * @param send_buffer Shared pointer to the send buffer.
 */
NetworkWorker::NetworkWorker(
    std::shared_ptr<ConnectionManager> connection_manager,
    std::shared_ptr<SocketTransport> socket_transport,
    std::shared_ptr<MessageProtocol> message_protocol,
    std::shared_ptr<MessageBuffer<std::string>> send_buffer
)   : connection_manager_(connection_manager),
      socket_transport_(socket_transport),
      message_protocol_(message_protocol),
      send_buffer_(send_buffer),
      running_(false)
{
    start();
}

/**
 * @brief Destroy the NetworkWorker object and stop the worker thread.
 */
NetworkWorker::~NetworkWorker()
{
    stop();
}

/**
 * @brief Start the network worker thread.
 */
void NetworkWorker::start()
{
    if (!running_.load()) {
        running_.store(true);
        worker_thread_ = std::thread(&NetworkWorker::worker_loop, this);
    }
}

/**
 * @brief Stop the network worker thread.
 */
void NetworkWorker::stop() {
    running_.store(false);
    if (worker_thread_.joinable()) {
        worker_thread_.join();
    }
}

/**
 * @brief Main loop for the network worker thread. Handles receiving and sending messages.
 */
void NetworkWorker::worker_loop() {
    while (running_.load()) {
        if (!connection_manager_->isConnected()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        std::string received = socket_transport_->receive_raw();
        if (!received.empty()) {
            message_protocol_->process_received_data(received);
        }

        std::string to_send;
        if (send_buffer_->tryPop(to_send)) {
            std::string formatted =
                message_protocol_->format_outgoing_message(to_send);
            if (!socket_transport_->send_raw(formatted)) {
                send_buffer_->pushFront(to_send);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
