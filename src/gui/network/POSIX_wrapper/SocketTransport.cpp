/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** SocketTransport
*/

#include "SocketTransport.hpp"
#include <sys/socket.h>
#include <errno.h>

SocketTransport::SocketTransport(
    std::shared_ptr<ConnectionManager> connection_manager)
    : connection_manager_(connection_manager) {
}

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
