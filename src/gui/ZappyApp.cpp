/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Application implementation
*/

#include "ZappyApp.hpp"

ZappyApp::ZappyApp() : OgreBites::ApplicationContext("Zappy GUI")
{
}

ZappyApp::~ZappyApp()
{
}

void ZappyApp::setup()
{
    OgreBites::ApplicationContext::setup();
    _scene.CreateScenne("ZappyWorld");
    _scene.Initialize(this);
}

Scenne& ZappyApp::getScene()
{
    return _scene;
}
