/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TeamManager
*/

#include "Team.hpp"
#include "entity/Entity.hpp"
#include "entity/Player.hpp"
#include <cmath>
#include <random>

/**
 * @brief Generate a consistent color for a team based on its name
 *
 * @param team_name The name of the team
 * @return Color A color object with RGB values for the team
 */
Color TeamManager::generateTeamColor(const std::string &team_name)
{
    // Create a simple hash from the team name
    unsigned int hash = 0;
    for (char c : team_name) {
        hash = hash * 31 + c;
    }
    std::mt19937 generator(hash);
    std::uniform_real_distribution<float> distribution(0.3f, 0.9f);
    float r = distribution(generator);
    float g = distribution(generator);
    float b = distribution(generator);
    if (r < 0.7f && g < 0.7f && b < 0.7f) {
        int component = hash % 3;
        if (component == 0)
            r = 0.9f;
        else if (component == 1)
            g = 0.9f;
        else
            b = 0.9f;
    }

    return Color(r, g, b, 1.0f);
}

/**
 * @brief Set a color for a specific team and store it
 *
 * @param team_name The name of the team
 */
void TeamManager::setTeamColor(const std::string &team_name)
{
    if (_teamColors.find(team_name) == _teamColors.end()) {
        _teamColors[team_name] = generateTeamColor(team_name);
    }
}

/**
 * @brief Set colors for all players of a specific team
 *
 * @param team_name The name of the team
 * @param players Map of all players
 */
void TeamManager::setPlayersColor(const std::string &team_name,
                                  const PlayerMap &players)
{
    setTeamColor(team_name);
    Color teamColor = _teamColors[team_name];
    for (const auto &[playerId, player] : players) {
        if (player && player->getTeamName() == team_name) {
            player->setColor(teamColor);
        }
    }
}

/**
 * @brief Set colors for all players in all teams
 *
 * @param players Map of all players
 */
void TeamManager::setAllPlayersColors(const PlayerMap &players)
{
    std::set<std::string> teamNames;
    for (const auto &[playerId, player] : players) {
        if (player) {
            teamNames.insert(player->getTeamName());
        }
    }
    for (const auto &teamName : teamNames) {
        setPlayersColor(teamName, players);
    }
}

/**
 * @brief Get the color assigned to a team
 *
 * @param team_name The name of the team
 * @return Color The color assigned to the team (or default if not found)
 */
Color TeamManager::getTeamColor(const std::string &team_name) const
{
    auto it = _teamColors.find(team_name);
    if (it != _teamColors.end()) {
        return it->second;
    }
    return Color(1.0f, 1.0f, 1.0f, 1.0f);
}