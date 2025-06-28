/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Application implementation
*/

#include "ZappyApp.hpp"
#include <iostream>

/**
 * @brief Construct a new ZappyApp object.
 */
ZappyApp::ZappyApp() : OgreBites::ApplicationContext("Zappy GUI") {}

/**
 * @brief Destroy the ZappyApp object.
 */
ZappyApp::~ZappyApp() {}

/**
 * @brief Set up the Zappy application and scene.
 */
void ZappyApp::setup() {
  OgreBites::ApplicationContext::setup();
  Ogre::Root *root = getRoot();
  if (!root) {
    std::cerr << "Error: Ogre Root is null" << std::endl;
    return;
  }
  Ogre::RenderSystem *rs = root->getRenderSystem();
  if (!rs) {
    std::cerr << "Error: No render system available" << std::endl;
    return;
  }
  Ogre::RenderWindow *window = getRenderWindow();
  if (!window) {
    std::cerr << "Error: No render window available" << std::endl;
    return;
  }
  std::cout << "Creating and initializing scene..." << std::endl;
  _scene.Initialize(this);
}

/**
 * @brief Get the scene object.
 *
 * @return Scene& Reference to the scene.
 */
Scene &ZappyApp::getScene() { return _scene; }
