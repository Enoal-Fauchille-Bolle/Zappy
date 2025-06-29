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
#include <map>
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
  void updateResourceCounts();
  void setStatusText(const std::string &text);
  void show();
  void hide();
  void updateTeams();

private:
  Ogre::Overlay *mOverlay;
  Ogre::OverlayContainer *mPanel;
  Ogre::TextAreaOverlayElement *mStatusText;
  Ogre::TextAreaOverlayElement *mTickRateText;

  // Resource text elements
  Ogre::TextAreaOverlayElement *mFoodText;
  Ogre::TextAreaOverlayElement *mLinemate;
  Ogre::TextAreaOverlayElement *mDeraumere;
  Ogre::TextAreaOverlayElement *mSibur;
  Ogre::TextAreaOverlayElement *mMendiane;
  Ogre::TextAreaOverlayElement *mPhiras;
  Ogre::TextAreaOverlayElement *mThystame;
  Ogre::TextAreaOverlayElement *mTeamsText; // Add this line

  SimpleGameManager *mGameManager;
  static const float UPDATE_INTERVAL;

  // Create a text area element with common properties
  Ogre::TextAreaOverlayElement *createTextElement(Ogre::OverlayManager *manager,
                                                  const std::string &name,
                                                  int yPosition);
};

#endif // OVERLAY_HPP