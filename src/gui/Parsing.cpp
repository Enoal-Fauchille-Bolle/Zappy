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

/**
 * @brief Constructor for the Parser class.
 *
 * Initializes the parser with command line arguments and validates them.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 */
Parser::Parser(int argc, char **argv)
{
    if (argc < 3) {
        printUsage();
        exit(EXIT_FAILURE);
    }
    parseArgs(argc, argv);
    if (!isValidPort(port_)) {
        printError("Port must be specified");
    }
}

/**
 * @brief Gets the parsed port number.
 *
 * @return int The port number.
 */
int Parser::getPort()
{
    return port_;
}

/**
 * @brief Gets the parsed host name.
 *
 * @return std::string The host name.
 */
std::string Parser::getHost()
{
    return host_;
}

/**
 * @brief Parses command line arguments.
 *
 * Processes command line arguments to extract port and host information.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 */
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

/**
 * @brief Handles port argument parsing.
 *
 * Validates and sets the port number from command line arguments.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @param i Reference to current argument index.
 */
void Parser::handlePortArgument(int argc, char **argv, int &i)
{
    if (i + 1 < argc && isValidPort(std::stoi(argv[++i]))) {
        port_ = std::stoi(argv[i]);
    } else {
        printError("Invalid port number.");
    }
}

/**
 * @brief Handles host argument parsing.
 *
 * Validates and sets the host name from command line arguments.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @param i Reference to current argument index.
 */
void Parser::handleHostArgument(int argc, char **argv, int &i)
{
    if (i + 1 < argc && isValidHost(argv[++i])) {
        host_ = argv[i];
    } else {
        printError("Invalid host name.");
    }
}

/**
 * @brief Validates if a port number is within the valid range.
 *
 * @param port The port number to validate.
 * @return bool True if port is valid (1024-65535), false otherwise.
 */
bool Parser::isValidPort(const int &port)
{
    return port > 1024 && port <= 65535;
}

/**
 * @brief Validates if a host name is valid.
 *
 * Checks if the host is either "localhost" or a valid IP address.
 *
 * @param host The host name to validate.
 * @return bool True if host is valid, false otherwise.
 */
bool Parser::isValidHost(const std::string &host)
{
    if (host == "localhost") {
        return true;
    }

    size_t pos = 0;
    int count = 0;
    std::string token;
    std::string tempHost = host + ".";
    while ((pos = tempHost.find('.')) != std::string::npos) {
        token = tempHost.substr(0, pos);
        tempHost.erase(0, pos + 1);
        try {
            int num = std::stoi(token);
            if (num < 0 || num > 255) {
                return false;
            }
        } catch (...) {
            return false;
        }
        ++count;
    }
    return count == 4;
}

/**
 * @brief Prints usage information to console.
 *
 * Displays help information showing how to use the application.
 */
void Parser::printUsage() const
{
    std::cout << "Usage: ./zappy -p <port> -h <host>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -p, --port <port>   Specify the port number (1024-65535)." << std::endl;
    std::cout << "  -h, --host <host>   Specify the host name (default: localhost)." << std::endl;
}

/**
 * @brief Prints error message and exits application.
 *
 * Displays error message to stderr and terminates the program.
 *
 * @param message The error message to display.
 */
void Parser::printError(const std::string &message) const
{
    std::cerr << "Error: " << message << std::endl;
    std::cerr << "Use --help for usage information." << std::endl;
    exit(EXIT_FAILURE);
}

} // namespace gui
