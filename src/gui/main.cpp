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

#include <iostream>
#include <ostream>
#include <string>
#include "Parsing.hpp"
#include "ZappyApp.hpp"
#include "GameManager.hpp"
#include "network/NetworkManager.hpp"

/**
 * @brief Main entry point for the Zappy GUI application.
 *
 * Parses command line arguments, initializes the application, game manager, and main loop.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @return int Returns 0 on success, 84 on error.
 */
int main(int argc, char **argv)
{
    try {
        gui::Parser parser(argc, argv);

        // NetworkManager::initialize(parser.getHost(),
        //     parser.getPort());
        // NetworkManager::send("GRAPHICS");
        // std::string response = NetworkManager::receive();
        // std::cout << "response: " << response << std::endl;

        ZappyApp app;
        app.initApp();

        SimpleGameManager gameManager;
        gameManager.initialize(&app.getScene());
        gameManager.setMapSize(10, 10);
        gameManager.createPlayer(1, "TeamA", 0, 0, Orientation::NORTH);
        gameManager.createEgg(1, 0, 5, 5);

        gameManager.createResource(ResourceType::FOOD, 2, 2, 3);
        gameManager.createResource(ResourceType::LINEMATE, 2, 2, 2);
        gameManager.createResource(ResourceType::DERAUMERE, 4, 4, 3);
        gameManager.createResource(ResourceType::SIBUR, 5, 5, 2);
        gameManager.createResource(ResourceType::MENDIANE, 6, 6, 1);
        gameManager.createResource(ResourceType::PHIRAS, 7, 7, 4);
        gameManager.createResource(ResourceType::THYSTAME, 8, 8, 6);

        bool running = true;
        while (running && !app.getRoot()->endRenderingQueued()) {
            gameManager.update();
            app.getRoot()->renderOneFrame();
        }
        app.closeApp();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
}