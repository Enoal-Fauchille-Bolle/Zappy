/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** ConnectionManager
*/

#ifndef CONNECTIONMANAGER_HPP_
#define CONNECTIONMANAGER_HPP_

#include <atomic>
#include <string>
#include <sys/socket.h>
class ConnectionManager {
    private:
        static constexpr int SOCKET_TIMEOUT_MS = 5000;
        static constexpr int CONNECTION_TIMEOUT_MS = 10000;
        static constexpr int KEEPALIVE_IDLE_SEC = 30;
        static constexpr int KEEPALIVE_INTERVAL_SEC = 5;
        static constexpr int KEEPALIVE_COUNT = 3;
        static constexpr int BUFFER_SIZE = 65536; // 64Kb

        std::string host_;
        int port_;
        std::atomic<int> socket_fd_;
        std::atomic<bool> connected_;

        void setSocketTimeout();
        bool configureSocketOptions();
        bool setNonBlocking(bool non_block = false);
        bool connectWithTimeout(const struct sockaddr *addr, socklen_t len);

    public:
        ConnectionManager(const std::string& host, int port);
        ~ConnectionManager();

        bool connect();
        void disconnect();
        bool isConnected() const { return connected_.load(); }
        bool isSocketValid() const;
        // bool isSocketWriteable(int timeout_ms = 0) const;
        // bool isSocketReadable(int timeout_ms = 0) const;
        int getSocketFd() const { return socket_fd_.load(); }

        bool testConnection() const;
        std::string getConnectionInfo() const;
        // bool getSocketError(int &error) const;

        const std::string& getHost() const { return host_; }
        int getPort() const { return port_; }
};

#endif /* !CONNECTIONMANAGER_HPP_ */
