/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** ConnectionManager
*/

#include "ConnectionManager.hpp"
#include <asm-generic/socket.h>
#include <bits/types/struct_timeval.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

ConnectionManager::ConnectionManager(const std::string &host, int port)
    : host_(host),
      port_(port),
      socket_fd_(-1),
      connected_(false)
{
    connect();
}

ConnectionManager::~ConnectionManager()
{
   disconnect();
}

bool ConnectionManager::connect()
{
    if (socket_fd_.load() != -1) {
        disconnect();
    }

    int new_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (new_socket < 0) {
        std::cerr << "Failed to create socket: " << strerror(errno) << std::endl;
        return false;
    }
    socket_fd_.store(new_socket);

    if (!configureSocketOptions()) {
        close(socket_fd_.load());
        socket_fd_.store(-1);
        return false;
    }

    struct sockaddr_in server_addr = {AF_INET, htons(port_),
        0, 0};
    if (inet_pton(AF_INET, host_.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address: " << host_ << std::endl;
        close(socket_fd_.load());
        socket_fd_.store(-1);
        return false;
    }

    if (!connectWithTimeout((struct sockaddr *)&server_addr,
        sizeof(server_addr))) {
        std::cerr << "Connection failed: " << strerror(errno) << std::endl;
        close(socket_fd_.load());
        socket_fd_.store(-1);
        return false;
    }

    setSocketTimeout();
    connected_.store(true);
    std::cout << "Connected to " << host_ << ":" << port_ << std::endl;
    return true;
}

void ConnectionManager::disconnect()
{
    int current_socket = socket_fd_.load();
    if (current_socket != -1) {
        connected_.store(false);

        shutdown(current_socket, SHUT_WR);

        char buffer[1024];
        struct pollfd pfd = {current_socket, POLLIN, 0};
        while (poll(&pfd, 1, 100) > 0 && (pfd.revents & POLLIN)) {
            if (recv(current_socket, buffer, sizeof(buffer), 0) <= 0) {
                break;
            }
        }

        close(current_socket);
        socket_fd_.store(-1);
        std::cout << "Disconnected from server" << std::endl;
    }
}

void ConnectionManager::setSocketTimeout()
{
    struct timeval timeout = {SOCKET_TIMEOUT_MS / 1000,
        (SOCKET_TIMEOUT_MS % 1000) * 1000};

    int current_socket = socket_fd_.load();
    setsockopt(current_socket, SOL_SOCKET, SO_RCVTIMEO,
        &timeout, sizeof(timeout));
    setsockopt(current_socket, SOL_SOCKET, SO_SNDTIMEO,
        &timeout, sizeof(timeout));
}

bool ConnectionManager::configureSocketOptions()
{
    int set_true = 1;
    int current_socket = socket_fd_.load();

    if (setsockopt(current_socket, SOL_SOCKET, SO_REUSEADDR,
        &set_true, sizeof(set_true)) < 0) {
        std::cerr << "Failed to set reuse address: "
                  << strerror(errno) << std::endl;
        return false;
    }

    int keepalive_idle = KEEPALIVE_IDLE_SEC;
    int keepalive_interval = KEEPALIVE_INTERVAL_SEC;
    int keepalive_count = KEEPALIVE_COUNT;

    if (setsockopt(current_socket, SOL_SOCKET, SO_KEEPALIVE,
        &set_true, sizeof(set_true)) < 0) {
        std::cerr << "Failed to set keep alive: "
                  << strerror(errno) << std::endl;
    }
    setsockopt(current_socket, IPPROTO_TCP, TCP_KEEPIDLE,
        &keepalive_idle, sizeof(keepalive_idle));
    setsockopt(current_socket, IPPROTO_TCP, TCP_KEEPINTVL,
        &keepalive_interval, sizeof(keepalive_interval));
    setsockopt(current_socket, IPPROTO_TCP, TCP_KEEPCNT,
        &keepalive_count, sizeof(keepalive_count));

    if (setsockopt(current_socket, IPPROTO_TCP, TCP_NODELAY,
        &set_true, sizeof(set_true)) < 0) {
        std::cerr << "Failed to set no delay: "
                  << strerror(errno) << std::endl;
    }

    int send_buff = BUFFER_SIZE;
    int recv_buf = BUFFER_SIZE;
    setsockopt(current_socket, SOL_SOCKET, SO_SNDBUF,
        &send_buff, sizeof(send_buff));
    setsockopt(current_socket, SOL_SOCKET, SO_RCVBUF,
        &recv_buf, sizeof(recv_buf));

    setsockopt(current_socket, SOL_SOCKET, SO_OOBINLINE,
        &set_true, sizeof(set_true));

    return true;
}

bool ConnectionManager::connectWithTimeout(
    const struct sockaddr *addr, socklen_t len)
{
    if (!setNonBlocking(true)) {
        return false;
    }

    int current_socket = socket_fd_.load();
    if (::connect(current_socket, addr, len) == 0) {
        setNonBlocking(false);
        return true;
    }

    if (errno != EINPROGRESS) {
        return false;
    }

    struct pollfd pfd = {current_socket, POLLIN | POLLOUT, 0};
    if (poll(&pfd, 1, CONNECTION_TIMEOUT_MS) <= 0) {
        return false;
    }
    if (pfd.revents & (POLLERR | POLLHUP)) {
        return false;
    }
    if ((pfd.revents & POLLIN) && !(pfd.revents & POLLOUT)) {
        return false;
    }

    int error = 0;
    socklen_t error_len = sizeof(error);
    if (getsockopt(current_socket, SOL_SOCKET, SO_ERROR,
        &error, &error_len) < 0) {
        return false;
    }
    if (error) {
        errno = error;
        return false;
    }

    setNonBlocking(false);
    return true;
}

bool ConnectionManager::setNonBlocking(bool non_block)
{
    int current_socket = socket_fd_.load();
    int flags = fcntl(current_socket, F_GETFL, 0);

    if (flags == -1) {
        return false;
    }

    if (non_block) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~O_NONBLOCK;
    }

    return fcntl(current_socket, F_SETFL, flags) != -1;
}

bool ConnectionManager::isSocketValid() const
{
    int current_socket = socket_fd_.load();
    if (current_socket == -1) {
        return false;
    }

    int error = 0;
    socklen_t error_len = sizeof(error);
    if (getsockopt(current_socket, SOL_SOCKET, SO_ERROR,
        &error, &error_len) < 0) {
        return false;
    }
    if (error) {
        errno = error;
        return false;
    }

    struct pollfd pfd = {current_socket, 0, 0};
    if (poll(&pfd, 1, 0) < 0) {
        return false;
    }
    if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) {
        return false;
    }

    return true;
}

bool ConnectionManager::testConnection() const
{
    if (!isSocketValid()) {
        return false;
    }

    int current_socket = socket_fd_.load();
    return send(current_socket, nullptr, 0,
        MSG_NOSIGNAL | MSG_DONTWAIT) == 0;
}

std::string ConnectionManager::getConnectionInfo() const
{
    std::string info = "Connection: ";
    int current_socket = socket_fd_.load();

    if (current_socket == -1) {
        return "No connection";
    }

    struct sockaddr_in local_addr, remote_addr;
    socklen_t addr_len = sizeof(local_addr);

    if (getsockname(current_socket,
        (struct sockaddr *)&local_addr, &addr_len) == 0) {
        char local_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &local_addr.sin_addr, local_ip, INET_ADDRSTRLEN);
        info += std::string(local_ip) + ":" +
            std::to_string(ntohs(local_addr.sin_port));
    }
    info += "->";

    addr_len = sizeof(remote_addr);
    if (getpeername(current_socket,
        (struct sockaddr *)&remote_addr, &addr_len) == 0) {
        char remote_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &remote_addr.sin_addr, remote_ip, INET_ADDRSTRLEN);
        info += std::string(remote_ip) + ":" +
            std::to_string(ntohs(remote_addr.sin_port));
    }

    return info;
}
