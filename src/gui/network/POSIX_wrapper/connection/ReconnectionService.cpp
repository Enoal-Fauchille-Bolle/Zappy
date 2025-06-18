/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** ReconnectionService
*/

#include "ReconnectionService.hpp"

/**
 * @brief Construct a new ReconnectionService object.
 * @param connection_manager Shared pointer to the ConnectionManager.
 */
ReconnectionService::ReconnectionService(
    std::shared_ptr<ConnectionManager> connection_manager)
    : connection_manager_(connection_manager), running_(false)
{
}

/**
 * @brief Destroy the ReconnectionService object and stop the reconnection thread.
 */
ReconnectionService::~ReconnectionService()
{
    stop();
}

/**
 * @brief Start the reconnection monitoring thread.
 */
void ReconnectionService::start()
{
    if (!running_.load()) {
        running_.store(true);
        connection_thread_ = std::thread(
            &ReconnectionService::connection_monitor, this);
    }
}

/**
 * @brief Stop the reconnection monitoring thread.
 */
void ReconnectionService::stop()
{
    running_.store(false);
    if (connection_thread_.joinable()) {
        connection_thread_.join();
    }
}

/**
 * @brief Monitor the connection and attempt reconnection if disconnected.
 */
void ReconnectionService::connection_monitor()
{
    while (running_.load()) {
        if (!connection_manager_->isConnected() ||
            !connection_manager_->testConnection()) {
            connection_manager_->disconnect();

            while (running_.load() && !connection_manager_->connect()) {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(RECONNECT_DELAY_MS));
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
