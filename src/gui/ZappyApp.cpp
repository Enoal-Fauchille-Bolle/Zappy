/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Application implementation
*/

#include "ZappyApp.hpp"

/**
 * @brief Construct a new ZappyApp object.
 */
ZappyApp::ZappyApp() : OgreBites::ApplicationContext("Zappy GUI")
{
}

/**
 * @brief Destroy the ZappyApp object.
 */
ZappyApp::~ZappyApp()
{
}

/**
 * @brief Set up the Zappy application and scene.
 */
void ZappyApp::setup()
{
    OgreBites::ApplicationContext::setup();
    _scene.CreateScenne("ZappyWorld");
    _scene.Initialize(this);
}

/**
 * @brief Get the scene object.
 *
 * @return Scenne& Reference to the scene.
 */
Scenne& ZappyApp::getScene()
{
    return _scene;
}
