/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** ReconnectionService
*/

#ifndef RECONNECTIONSERVICE_HPP_
#define RECONNECTIONSERVICE_HPP_

#include <memory>
#include <thread>
#include "ConnectionManager.hpp"


class ReconnectionService {
private:
    static constexpr int RECONNECT_DELAY_MS = 1000;

    std::shared_ptr<ConnectionManager> connection_manager_;
    std::atomic<bool> running_;
    std::thread connection_thread_;

    void connection_monitor();

public:
    explicit ReconnectionService(std::shared_ptr<ConnectionManager> connection_manager);
    ~ReconnectionService();

    void start();
    void stop();
};

#endif /* !RECONNECTIONSERVICE_HPP_ */
