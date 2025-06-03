/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Gui Argument Parser
*/

#ifndef GUI_PARSING_HPP_
#define GUI_PARSING_HPP_

#include <string>
#include <vector>

namespace gui {

    class Parser {
    public:
        Parser(int argc, char **argv);
        ~Parser() = default;

        int getPort();
        std::string getHost();

    private:
        int port_;
        std::string host_ = "localhost"; // Default host

        void parseArgs(int argc, char **argv);

        void handlePortArgument(int argc, char **argv, int &i);
        void handleHostArgument(int argc, char **argv, int &i);

        static bool isValidPort(const int &port);
        static bool isValidHost(const std::string &host);

        void printUsage() const;
        void printError(const std::string &message) const;
    };
}

#endif // GUI_PARSING_HPP_