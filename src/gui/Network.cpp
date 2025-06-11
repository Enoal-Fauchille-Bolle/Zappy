/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network
*/

#include "Network.hpp"
#include "Network_encapsulation.hpp"
#include <iostream>

Network::Network()
{
    _CallNetwork = Network_encapsulation();
    _map = {0, 0};
    _teamName = "";
    _playerName = "";
    _playerId = -1;
    _sockfd = -1;
    initMessageHandlers();
}

Network::~Network()
{
}

void Network::CreateConection(const char *ip, const char *port)
{
    _CallNetwork.connect_to_server(ip, port);
    if (!_CallNetwork.isConnected()) {
        std::cerr << "Failed to connect to server at " << ip << ":" << port << std::endl;
        return;
    }
    std::cout << "Connected to server at " << ip << ":" << port << std::endl;
}

void Network::sendMsg(const std::string &msg)
{
    if (_CallNetwork.isConnected()) {
        _CallNetwork.sendMsg(msg);
    } else {
        std::cerr << "Not connected to server. Cannot send message." << std::endl;
    }
}

void Network::requestMapSize()
{
    sendMsg("msz\n");
}

void Network::requestTileContent(int x, int y)
{
    sendMsg("bct " + std::to_string(x) + " " + std::to_string(y) + "\n");
}

void Network::requestAllTilesContent()
{
    sendMsg("mct\n");
}

void Network::requestTeamNames()
{
    sendMsg("tna\n");
}

void Network::requestPlayerPosition(int playerId)
{
    sendMsg("ppo #" + std::to_string(playerId) + "\n");
}

void Network::requestPlayerLevel(int playerId)
{
    sendMsg("plv #" + std::to_string(playerId) + "\n");
}

void Network::requestPlayerInventory(int playerId)
{
    sendMsg("pin #" + std::to_string(playerId) + "\n");
}

void Network::requestTimeUnit()
{
    sendMsg("sgt\n");
}

void Network::setTimeUnit(int timeUnit)
{
    if (timeUnit > 0) {
        sendMsg("sst " + std::to_string(timeUnit) + "\n");
    }
}

std::string Network::receiveMsg()
{
    if (_CallNetwork.isConnected()) {
        return _CallNetwork.getMsg();
    }
    return "";
}

// Initialize message handlers
void Network::initMessageHandlers()
{
    _messageHandlers["msz"] = &Network::handleMapSize;
    _messageHandlers["bct"] = &Network::handleTileContent;
    _messageHandlers["tna"] = &Network::handleTeamName;
    _messageHandlers["pnw"] = &Network::handleNewPlayer;
    _messageHandlers["ppo"] = &Network::handlePlayerPosition;
    _messageHandlers["plv"] = &Network::handlePlayerLevel;
    _messageHandlers["pin"] = &Network::handlePlayerInventory;
    _messageHandlers["sgt"] = &Network::handleTimeUnit;
    _messageHandlers["seg"] = &Network::handleGameEnd;
    _messageHandlers["smg"] = &Network::handleServerMessage;
    _messageHandlers["suc"] = &Network::handleUnknownCommand;
}

// Simple message parser to handle server responses
void Network::processServerResponse(const std::string &response)
{
    if (response.empty())/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Network
*/

#include "Network.hpp"
#include "Network_encapsulation.hpp"
#include <iostream>

Network::Network()
{
    _CallNetwork = Network_encapsulation();
    _map = {0, 0};
    _teamName = "";
    _playerName = "";
    _playerId = -1;
    _sockfd = -1;
    initMessageHandlers();
}

Network::~Network()
{
}

void Network::CreateConection(const char *ip, const char *port)
{
    _CallNetwork.connect_to_server(ip, port);
    if (!_CallNetwork.isConnected()) {
        std::cerr << "Failed to connect to server at " << ip << ":" << port << std::endl;
        return;
    }
    std::cout << "Connected to server at " << ip << ":" << port << std::endl;
}

void Network::sendMsg(const std::string &msg)
{
    if (_CallNetwork.isConnected()) {
        _CallNetwork.sendMsg(msg);
    } else {
        std::cerr << "Not connected to server. Cannot send message." << std::endl;
    }
}

void Network::requestMapSize()
{
    sendMsg("msz\n");
}

void Network::requestTileContent(int x, int y)
{
    sendMsg("bct " + std::to_string(x) + " " + std::to_string(y) + "\n");
}

void Network::requestAllTilesContent()
{
    sendMsg("mct\n");
}

void Network::requestTeamNames()
{
    sendMsg("tna\n");
}

void Network::requestPlayerPosition(int playerId)
{
    sendMsg("ppo #" + std::to_string(playerId) + "\n");
}

void Network::requestPlayerLevel(int playerId)
{
    sendMsg("plv #" + std::to_string(playerId) + "\n");
}

void Network::requestPlayerInventory(int playerId)
{
    sendMsg("pin #" + std::to_string(playerId) + "\n");
}

void Network::requestTimeUnit()
{
    sendMsg("sgt\n");
}

void Network::setTimeUnit(int timeUnit)
{
    if (timeUnit > 0) {
        sendMsg("sst " + std::to_string(timeUnit) + "\n");
    }
}

std::string Network::receiveMsg()
{
    if (_CallNetwork.isConnected()) {
        return _CallNetwork.getMsg();
    }
    return "";
}

// Initialize message handlers
void Network::initMessageHandlers()
{
    _messageHandlers["msz"] = &Network::handleMapSize;
    _messageHandlers["bct"] = &Network::handleTileContent;
    _messageHandlers["tna"] = &Network::handleTeamName;
    _messageHandlers["pnw"] = &Network::handleNewPlayer;
    _messageHandlers["ppo"] = &Network::handlePlayerPosition;
    _messageHandlers["plv"] = &Network::handlePlayerLevel;
    _messageHandlers["pin"] = &Network::handlePlayerInventory;
    _messageHandlers["sgt"] = &Network::handleTimeUnit;
    _messageHandlers["seg"] = &Network::handleGameEnd;
    _messageHandlers["smg"] = &Network::handleServerMessage;
    _messageHandlers["suc"] = &Network::handleUnknownCommand;
}

// Simple message parser to handle server responses
void Network::processServerResponse(const std::string &response)
{
    if (response.empty())
        return;
    // Extract command prefix (first 3 characters)
    std::string prefix = response.substr(0, 3);

    auto it = _messageHandlers.find(prefix);
    if (it != _messageHandlers.end()) {
        // Call the appropriate handler method
        (this->*(it->second))(response);
    } else {
        std::cout << "Unknown command: " << prefix << std::endl;
    }
}

// Individual message handlers
void Network::handleMapSize(const std::string &msg)
{
    std::cout << "Received map size information" << std::endl;
    // Parse: msz X Y
    // TODO: Implement actual parsing
}

void Network::handleTileContent(const std::string &msg)
{
    std::cout << "Received tile content information" << std::endl;
    // Parse: bct X Y q0 q1 q2 q3 q4 q5 q6
    // TODO: Implement actual parsing
}

void Network::handleTeamName(const std::string &msg)
{
    std::cout << "Received team name" << std::endl;
    // Parse: tna N
    // TODO: Implement actual parsing
}

void Network::handleNewPlayer(const std::string &msg)
{
    std::cout << "New player connected" << std::endl;
    // Parse: pnw #n X Y O L N
    // TODO: Implement actual parsing
}

void Network::handlePlayerPosition(const std::string &msg)
{
    std::cout << "Received player position" << std::endl;
    // Parse: ppo #n X Y O
    // TODO: Implement actual parsing
}

void Network::handlePlayerLevel(const std::string &msg)
{
    std::cout << "Received player level" << std::endl;
    // Parse: plv #n L
    // TODO: Implement actual parsing
}

void Network::handlePlayerInventory(const std::string &msg)
{
    std::cout << "Received player inventory" << std::endl;
    // Parse: pin #n X Y q0 q1 q2 q3 q4 q5 q6
    // TODO: Implement actual parsing
}

void Network::handleTimeUnit(const std::string &msg)
{
    std::cout << "Received time unit information" << std::endl;
    // Parse: sgt T
    // TODO: Implement actual parsing
}

void Network::handleGameEnd(const std::string &msg)
{
    std::cout << "Game ended" << std::endl;
    // Parse: seg N
    // TODO: Implement actual parsing
}

void Network::handleServerMessage(const std::string &msg)
{
    std::cout << "Server message: " << msg.substr(4) << std::endl;
    // Parse: smg M
}

void Network::handleUnknownCommand(const std::string &msg)
{
    std::cout << "Server responded with unknown command" << std::endl;
    // Parse: suc
}

// Update method to regularly check for and process messages
void Network::update()
{
    std::string response = receiveMsg();
    if (!response.empty()) {
        processServerResponse(response);
    }
}


        return;
    // Extract command prefix (first 3 characters)
    std::string prefix = response.substr(0, 3);

    auto it = _messageHandlers.find(prefix);
    if (it != _messageHandlers.end()) {
        // Call the appropriate handler method
        (this->*(it->second))(response);
    } else {
        std::cout << "Unknown command: " << prefix << std::endl;
    }
}

// Individual message handlers
void Network::handleMapSize(const std::string &msg)
{
    std::cout << "Received map size information" << std::endl;
}

void Network::handleTileContent(const std::string &msg)
{
    std::cout << "Received tile content information" << std::endl;
}

void Network::handleTeamName(const std::string &msg)
{
    std::cout << "Received team name" << std::endl;
}

void Network::handleNewPlayer(const std::string &msg)
{
    std::cout << "New player connected" << std::endl;
}

void Network::handlePlayerPosition(const std::string &msg)
{
    std::cout << "Received player position" << std::endl;
}

void Network::handlePlayerLevel(const std::string &msg)
{
    std::cout << "Received player level" << std::endl;
}

void Network::handlePlayerInventory(const std::string &msg)
{
    std::cout << "Received player inventory" << std::endl;
}

void Network::handleTimeUnit(const std::string &msg)
{
    std::cout << "Received time unit information" << std::endl;
}

void Network::handleGameEnd(const std::string &msg)
{
    std::cout << "Game ended" << std::endl;
}

void Network::handleServerMessage(const std::string &msg)
{
    std::cout << "Server message: " << msg.substr(4) << std::endl;
}

void Network::handleUnknownCommand(const std::string &msg)
{
    std::cout << "Server responded with unknown command" << std::endl;
}

// Update method to regularly check for and process messages
void Network::update()
{
    std::string response = receiveMsg();
    if (!response.empty()) {
        processServerResponse(response);
    }
}

