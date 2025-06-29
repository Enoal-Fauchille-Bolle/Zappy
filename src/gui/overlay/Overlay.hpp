/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Overlay
*/

#ifndef OVERLAY_HPP
    #define OVERLAY_HPP

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Ogre.h>
#include <OGRE/Overlay/OgreOverlay.h>
#include <OGRE/Overlay/OgreOverlayContainer.h>
#include <OGRE/Overlay/OgreOverlayManager.h>
#include <OGRE/Overlay/OgreOverlaySystem.h>
#include <OGRE/Overlay/OgreTextAreaOverlayElement.h>
#include <string>

class SimpleGameManager;

class Overlay {
public:
  Overlay();
  ~Overlay();

  void initialize(Ogre::SceneManager *sceneManager,
                  Ogre::OverlaySystem *overlaySystem);
  void setGameManager(SimpleGameManager *gameManager);
  void updateTickRate();
  void updateTeamText();
  void setStatusText(const std::string &text);
  void show();
  void hide();

private:
  Ogre::Overlay *mOverlay;
  Ogre::OverlayContainer *mPanel;
  Ogre::TextAreaOverlayElement *mStatusText;
  Ogre::TextAreaOverlayElement *mTickRateText;
  Ogre::TextAreaOverlayElement *mTeamText;
  SimpleGameManager *mGameManager;
  static const float UPDATE_INTERVAL;
};

#endif // OVERLAY_HPP