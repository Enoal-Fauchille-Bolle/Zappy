/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network_encapsulation
*/

#ifndef NETWORK_ENCAPSULATION_HPP_
#define NETWORK_ENCAPSULATION_HPP_

#include <string>
#include <netinet/in.h>

class Network_encapsulation {
    public:
        Network_encapsulation();
        Network_encapsulation(const char *ip, const char *port);
        ~Network_encapsulation();

        void connect_to_server(const char *ip, const char *port);
        std::string getMsg();
        void sendMsg(const std::string &msg);
        bool isConnected() const;

    protected:
    private:
        int _sockfd;
        struct sockaddr_in _serverAddr;
        bool _connected;
};

#endif /* !NETWORK_ENCAPSULATION_HPP_ */
