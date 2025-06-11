/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GUI Main File
*/

#include <iostream>
#include <OGRE/Ogre.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include "Parsing.hpp"
#include "scenne/Scenne.hpp"
#include "GameManager.hpp"

class ZappyApp : public OgreBites::ApplicationContext {
public:
    ZappyApp() : OgreBites::ApplicationContext("Zappy GUI") {}
    
    void setup() override
    {
        // Base setup
        OgreBites::ApplicationContext::setup();
        
        // Initialize scene
        _scene.CreateScenne("ZappyWorld");
        _scene.Initialize(this);
    }
    
    // Access to scene
    Scenne& getScene() { return _scene; }
    
private:
    Scenne _scene;
};

int main(int argc, char **argv)
{
    try {
        // Parse command line arguments
        gui::Parser parser(argc, argv);
        
        ZappyApp app;
        app.initApp();
        
        // Initialize game manager
        GameManager gameManager;
        gameManager.initialize(&app.getScene());
        gameManager.setMapSize(10, 10); // Default size
        
        
        bool running = true;
        while (running && !app.getRoot()->endRenderingQueued()) {
            // Process network messages
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