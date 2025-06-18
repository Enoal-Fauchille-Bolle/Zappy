/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** SocketTransport
*/

/**
 * @file SocketTransport.cpp
 * @brief Implementation of the SocketTransport class for low-level socket send/receive operations.
 */

#include "SocketTransport.hpp"
#include <sys/socket.h>
#include <errno.h>

/**
 * @brief Construct a new SocketTransport object.
 * @param connection_manager Shared pointer to the ConnectionManager.
 */
SocketTransport::SocketTransport(
    std::shared_ptr<ConnectionManager> connection_manager)
    : connection_manager_(connection_manager) {
}

/**
 * @brief Send raw data over the socket.
 * @param data The data string to send.
 * @return true if all data was sent successfully, false otherwise.
 */
bool SocketTransport::send_raw(const std::string& data) {
    if (!connection_manager_->isConnected()) {
        return false;
    }

    int socket_fd = connection_manager_->getSocketFd();
    ssize_t bytes_sent = send(socket_fd,
        data.c_str(), data.length(), MSG_NOSIGNAL);

    if (bytes_sent < 0) {
        if (errno != EAGAIN && errno != EWOULDBLOCK) {
            connection_manager_->disconnect();
        }
        return false;
    }

    return bytes_sent == static_cast<ssize_t>(data.length());
}

/**
 * @brief Receive raw data from the socket.
 * @return The received data as a string, or an empty string if no data is available or on error.
 */
std::string SocketTransport::receive_raw() {
    if (!connection_manager_->isConnected()) {
        return "";
    }

    int socket_fd = connection_manager_->getSocketFd();
    char buffer[RECV_BUFFER_SIZE];
    ssize_t bytes_received = recv(socket_fd,
        buffer, sizeof(buffer) - 1, MSG_DONTWAIT);

    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        return std::string(buffer);
    } else if (bytes_received == 0) {
        connection_manager_->disconnect();
    } else if (errno != EAGAIN && errno != EWOULDBLOCK) {
        connection_manager_->disconnect();
    }

    return "";
}
