/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Simplified Game Manager class
*/

#ifndef SIMPLE_GAME_MANAGER_HPP_
#define SIMPLE_GAME_MANAGER_HPP_

#include "Types.hpp"
#include "entity/Egg.hpp"
#include "entity/Player.hpp"
#include "entity/Resources.hpp"
#include "entity/TileDisplay.hpp"
#include "scene/Scene.hpp"
#include <map>
#include <unordered_set>
#include <vector>

class CommandHandler;
class Scene;

class SimpleGameManager {
public:
  SimpleGameManager();
  ~SimpleGameManager();

  void initialize(Scene *scene);
  void setMapSize(int width, int height);

  void createTile(int x, int y);

  void createPlayer(int id, const std::string &teamName, int x, int y,
                    Orientation orientation);
  void updatePlayerPosition(int id, int x, int y, Orientation orientation);
  void removePlayer(int id);
  PlayerMap getPlayers() const;

  void createEgg(int id, int parentId, int x, int y);
  void eggToPlayer(int eggId);
  void removeEgg(int id);

  void updateResource(ResourceType type, int x, int y, int quantity);
  void createResource(ResourceType type, int x, int y, int quantity);
  void removeResource(ResourceType type, int x, int y, int quantity);

  void setTickRate(int rate);
  int getTickRate() const;

  void addTeam(const std::string &teamName);
  std::unordered_set<std::string> getTeams() const;

  void update();
  void readResponse(const std::string &response);
  std::pair<int, int> getMapSize() const;

  void cleanup();

private:
  Scene *_scene;
  CommandHandler *_commandHandler;
  int _mapWidth, _mapHeight;

  int _tickRate;

  std::unordered_set<std::string> _teamNames;
  PlayerMap _players;
  EggMap _eggs;
  std::vector<std::vector<TileDisplay *>> _tiles;

  void createGrid();
  TileDisplay *getTile(int x, int y);
  bool isValidPosition(int x, int y) const;
  void positionPlayerOnTile(Player *player, int x, int y, bool animate = true);
  void positionResourceOnTile(Resources *resource, int x, int y, int index);
};

#endif /* !SIMPLE_GAME_MANAGER_HPP_ */
