/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI Main File
*/

#include <iostream>
#include "Parsing.hpp"
#include "ZappyApp.hpp"
#include "GameManager.hpp"

int main(int argc, char **argv)
{
    try {
        gui::Parser parser(argc, argv);

        ZappyApp app;
        app.initApp();

        GameManager gameManager;
        gameManager.initialize(&app.getScene());
        gameManager.setMapSize(10, 10);

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