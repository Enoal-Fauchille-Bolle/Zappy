/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Gui Argument Parser
*/

#include "Parsing.hpp"
#include <iostream>
#include <string>

namespace gui {

Parser::Parser(int argc, char **argv)
{
    if (argc < 3) {
        printUsage();
        exit(EXIT_FAILURE);
    }
    parseArgs(argc, argv);
}

int Parser::getPort()
{
    return port_;
}

std::string Parser::getHost()
{
    return host_;
}

void Parser::parseArgs(int argc, char **argv)
{
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-p" || arg == "--port") {
            handlePortArgument(argc, argv, i);
        } else if (arg == "-h" || arg == "--host") {
            handleHostArgument(argc, argv, i);
        } else if (arg == "--help") {
            printUsage();
            exit(EXIT_SUCCESS);
        } else {
            printError("Unknown argument: " + arg);
        }
    }
}

void Parser::handlePortArgument(int argc, char **argv, int &i)
{
    if (i + 1 < argc && isValidPort(std::stoi(argv[++i]))) {
        port_ = std::stoi(argv[i]);
    } else {
        printError("Invalid host name.");
    }
}

void Parser::handleHostArgument(int argc, char **argv, int &i)
{
    if (i + 1 < argc && isValidHost(argv[++i])) {
        host_ = argv[i];
    } else {
        printError("Invalid host name.");
    }
}

bool Parser::isValidPort(const int &port)
{
    return port > 1024 && port <= 65535;
}

bool Parser::isValidHost(const std::string &host)
{
    return !host.empty() && host.find_first_not_of("0123456789.") == std::string::npos;
}

void Parser::printUsage() const
{
    std::cout << "Usage: ./zappy -p <port> -h <host>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -p, --port <port>   Specify the port number (1024-65535)." << std::endl;
    std::cout << "  -h, --host <host>   Specify the host name (default: localhost)." << std::endl;
}

void Parser::printError(const std::string &message) const
{
    std::cerr << "Error: " << message << std::endl;
    std::cerr << "Use --help for usage information." << std::endl;
    exit(EXIT_FAILURE);
}

} // namespace gui
