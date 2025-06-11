/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network
*/

#ifndef NETWORK_HPP_
    #define NETWORK_HPP_

    #include "Network_encapsulation.hpp"
    #include <string>
    #include <sys/types.h>
    #include <unordered_map>
    #include <functional>

struct Map
{
    int width;
    int height;
};

// Define message handler function type
typedef void (Network::*MessageHandler)(const std::string &);

class Network {
    public:
        Network();
        ~Network();

        void CreateConection(const char *ip, const char *port);
        void sendMsg(const std::string &msg);

        // Map Information methods
        void requestMapSize();

        // Tile Information methods
        void requestTileContent(int x, int y);
        void requestAllTilesContent();

        // Team Information methods
        void requestTeamNames();

        // Player Information methods
        void requestPlayerPosition(int playerId);
        void requestPlayerLevel(int playerId);
        void requestPlayerInventory(int playerId);

        // Time Management methods
        void requestTimeUnit();
        void setTimeUnit(int timeUnit);

        // Message handling
        std::string receiveMsg();
        void processServerResponse(const std::string &response);
        void update();

    protected:

    private:
        // Message handler methods
        void handleMapSize(const std::string &msg);
        void handleTileContent(const std::string &msg);
        void handleTeamName(const std::string &msg);
        void handleNewPlayer(const std::string &msg);
        void handlePlayerPosition(const std::string &msg);
        void handlePlayerLevel(const std::string &msg);
        void handlePlayerInventory(const std::string &msg);
        void handleTimeUnit(const std::string &msg);
        void handleGameEnd(const std::string &msg);
        void handleServerMessage(const std::string &msg);
        void handleUnknownCommand(const std::string &msg);

        void initMessageHandlers();

        Network_encapsulation _CallNetwork;
        std::unordered_map<std::string, MessageHandler> _messageHandlers;

        Map _map;
        std::string _teamName;
        std::string _playerName;
        int _playerId;
        int _sockfd;
};

#endif /* !NETWORK_HPP_ */
