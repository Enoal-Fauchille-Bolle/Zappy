/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI Main File
*/

/**
 * @file main.cpp
 * @brief GUI Main File for Zappy project.
 *
 * This file contains the entry point for the GUI application.
 */

#include "GameManager.hpp"
#include "OGRE/OgreConfigFile.h"
#include "Parsing.hpp"
#include "ZappyApp.hpp"
#include "network/NetworkManager.hpp"
#include <chrono>
#include <iostream>
#include <ostream>
#include <string>

/**
 * @brief Main entry point for the Zappy GUI application.
 *
 * Parses command line arguments, initializes the application, game manager, and
 * main loop.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Returns 0 on success, 84 on error.
 */
int main(int argc, char **argv) {
  try {
    gui::Parser parser(argc, argv);

    NetworkManager::initialize(parser.getHost(), parser.getPort());
    NetworkManager::send("GRAPHIC");
    std::string response = NetworkManager::receive();

    ZappyApp app;
    app.initApp();

    SimpleGameManager gameManager;
    gameManager.initialize(&app.getScene());
    app.getScene().setGameManager(&gameManager);

    auto lastTime = std::chrono::high_resolution_clock::now();
    bool running = true;
    while (running && !app.getRoot()->endRenderingQueued()) {
      auto currentTime = std::chrono::high_resolution_clock::now();
      float deltaTime =
          std::chrono::duration<float>(currentTime - lastTime).count();
      lastTime = currentTime;

      app.getScene().Update(deltaTime, gameManager.getTickRate());
      gameManager.update();
      app.getRoot()->renderOneFrame();
    }
    gameManager.cleanup();
    app.closeApp();
    return 0;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 84;
  }
}