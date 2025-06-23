/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TeamManager
*/

#ifndef TEAM_MANAGER_HPP_
#define TEAM_MANAGER_HPP_

#include <string>
#include <map>
#include "Types.hpp"

class TeamManager {
    private:
        std::map<std::string, Color> _teamColors;
        Color generateTeamColor(const std::string &team_name);

    public:
        TeamManager() = default;
        ~TeamManager() = default;

        void setPlayersColor(const std::string &team_name,
                             const PlayerMap &players);
        void setAllPlayersColors(const PlayerMap &players);
        void setTeamColor(const std::string &team_name);
        Color getTeamColor(const std::string &team_name) const;
};

#endif  // TEAM_MANAGER_HPP_