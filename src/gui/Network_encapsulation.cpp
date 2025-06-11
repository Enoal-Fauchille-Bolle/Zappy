/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network_encapsulation
*/

#include "Network_encapsulation.hpp"

#include <unistd.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>

Network_encapsulation::Network_encapsulation() : _sockfd(-1), _connected(false)
{
}

Network_encapsulation::Network_encapsulation(const char *ip, const char *port) : _sockfd(-1), _connected(false)
{
    connect_to_server(ip, port);
}

Network_encapsulation::~Network_encapsulation()
{
    if (_sockfd != -1) {
        close(_sockfd);
    }
}

void Network_encapsulation::connect_to_server(const char *ip, const char *port)
{
    _sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (_sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_port = htons(std::stoi(port));
    if (inet_pton(AF_INET, ip, &_serverAddr.sin_addr) <= 0) {
        perror("inet_pton");
        close(_sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(_sockfd, reinterpret_cast<struct sockaddr *>(&_serverAddr),
                sizeof(_serverAddr)) == -1) {
        perror("connect");
        close(_sockfd);
        exit(EXIT_FAILURE);
    }

    _connected = true;
}

std::string Network_encapsulation::getMsg()
{
    if (!_connected || _sockfd == -1) {
        return "";
    }

    struct pollfd fds;
    fds.fd = _sockfd;
    fds.events = POLLIN;
    fds.revents = 0;

    int ret = poll(&fds, 1, 0);
    std::string message;

    if (ret > 0 && (fds.revents & POLLIN)) {
        char buffer[1024];
        std::memset(buffer, 0, sizeof(buffer));
        ssize_t bytesRead = read(_sockfd, buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            message.assign(buffer, static_cast<size_t>(bytesRead));
            std::cout << message << std::endl;
        }
    }
    return message;
}

void Network_encapsulation::sendMsg(const std::string &msg)
{
    if (!_connected || _sockfd == -1) {
        return;
    }

    const char *data = msg.c_str();
    size_t total = 0;
    size_t toSend = msg.length();

    while (total < toSend) {
        ssize_t sent = write(_sockfd, data + total, toSend - total);
        if (sent < 0) {
            perror("write");
            break;
        }
        total += static_cast<size_t>(sent);
    }
}

bool Network_encapsulation::isConnected() const
{
    return _connected;
}
