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
#include "Parsing.hpp"
#include "ZappyApp.hpp"
#include "GameManager.hpp"

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

        ZappyApp app;
        app.initApp();

        GameManager gameManager;
        gameManager.initialize(&app.getScene());
        gameManager.setMapSize(10, 10);
        gameManager.createPlayer(1, "TeamA", 0, 0, Orientation::SOUTH);

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