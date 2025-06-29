/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Overlay
*/

#include "Overlay.hpp"
#include "../GameManager.hpp"
#include <iostream>
#include <sstream>

const float Overlay::UPDATE_INTERVAL = 0.5f;

Overlay::Overlay()
    : mOverlay(nullptr), mPanel(nullptr), mStatusText(nullptr),
      mTickRateText(nullptr), mGameManager(nullptr) {}

Overlay::~Overlay() {
}

void Overlay::setGameManager(SimpleGameManager *gameManager) {
  mGameManager = gameManager;
  if (mGameManager && mTickRateText) {
    updateTickRate();
  }
}

void Overlay::initialize(Ogre::SceneManager *sceneManager,
                         Ogre::OverlaySystem *overlaySystem) {
  try {
    if (!overlaySystem) {
      std::cerr << "Error: Overlay system is null" << std::endl;
      return;
    }

    if (!sceneManager) {
      std::cerr << "Error: Scene manager is null, cannot add overlay system"
                << std::endl;
      return;
    }

    sceneManager->addRenderQueueListener(overlaySystem);

    std::cout << "Getting overlay manager..." << std::endl;
    Ogre::OverlayManager *overlayManager =
        Ogre::OverlayManager::getSingletonPtr();
    if (!overlayManager) {
      std::cerr << "Error: Failed to get overlay manager" << std::endl;
      return;
    }

    std::cout << "Creating overlay container..." << std::endl;
    try {
      mOverlay = overlayManager->create("GameOverlay");
      if (!mOverlay) {
        std::cerr << "Error: Failed to create overlay" << std::endl;
        return;
      }

      mOverlay->setZOrder(500);

      std::cout << "Creating panel..." << std::endl;
      mPanel = static_cast<Ogre::OverlayContainer *>(
          overlayManager->createOverlayElement("Panel", "GamePanel"));

      if (!mPanel) {
        std::cerr << "Error: Failed to create panel" << std::endl;
        return;
      }

      mPanel->setMetricsMode(Ogre::GMM_PIXELS);
      mPanel->setPosition(10, 10);
      mPanel->setDimensions(300, 120);

      std::cout << "Setting material for panel..." << std::endl;
      Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
          "GameOverlayMaterial",
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

      if (material) {
        material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
        material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
        mPanel->setMaterialName(material->getName());
      } else {
        mPanel->setMaterialName("BaseWhite");
        mPanel->setColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f, 0.7f));
      }

      try {
        mStatusText = static_cast<Ogre::TextAreaOverlayElement *>(
            overlayManager->createOverlayElement("TextArea", "StatusText"));
        mStatusText->setMetricsMode(Ogre::GMM_PIXELS);
        mStatusText->setPosition(20, 20);
        mStatusText->setDimensions(260, 40);
        mStatusText->setCaption("Zappy Game Status");
        mStatusText->setCharHeight(16);
        mStatusText->setFontName("SdkTrays/Value");
        mStatusText->setColour(Ogre::ColourValue::Black);
        mPanel->addChild(mStatusText);
        mTickRateText = static_cast<Ogre::TextAreaOverlayElement *>(
            overlayManager->createOverlayElement("TextArea", "TickRateText"));
        mTickRateText->setMetricsMode(Ogre::GMM_PIXELS);
        mTickRateText->setPosition(20, 60);
        mTickRateText->setDimensions(260, 40);
        mTickRateText->setCaption("Tick Rate: ---");
        mTickRateText->setCharHeight(16);
        mTickRateText->setFontName("SdkTrays/Value");
        mTickRateText->setColour(Ogre::ColourValue::Green);
        mPanel->addChild(mTickRateText);
        mTeamText = static_cast<Ogre::TextAreaOverlayElement *>(
            overlayManager->createOverlayElement("TextArea", "TeamText"));
        mTeamText->setMetricsMode(Ogre::GMM_PIXELS);
        mTeamText->setPosition(20, 100);
        mTeamText->setDimensions(260, 40);
        mTeamText->setCaption("Team: ---");
        mTeamText->setCharHeight(16);
        mTeamText->setFontName("SdkTrays/Value");
        mTeamText->setColour(Ogre::ColourValue::Black);
        mPanel->addChild(mTeamText);
      } catch (const Ogre::Exception &e) {
        std::cerr << "Failed to create text areas: " << e.what() << std::endl;
      }
      mOverlay->add2D(mPanel);
      mOverlay->show();
    } catch (const Ogre::Exception &e) {
      std::cerr << "Ogre Exception during overlay creation: " << e.what()
                << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard Exception during overlay creation: " << e.what()
                << std::endl;
    }
  } catch (const std::exception &e) {
    std::cerr << "Exception in initialize: " << e.what() << std::endl;
  }
}

void Overlay::updateTickRate() {
  static int warningCount = 0;
  if (!mGameManager) {
    warningCount++;
    return;
  }
  warningCount = 0;
  int tickRate = mGameManager->getTickRate();
  std::cout << "Current tick rate: " << tickRate << std::endl;
  std::stringstream ss;
  ss << "Tick Rate: " << tickRate;
  mTickRateText->setCaption(ss.str());
  if (tickRate < 10) {
    mTickRateText->setColour(
        Ogre::ColourValue(0.0f, 0.7f, 0.0f)); // Dark green (slow)
  } else if (tickRate < 50) {
    mTickRateText->setColour(
        Ogre::ColourValue(0.0f, 1.0f, 0.0f)); // Green (normal)
  } else if (tickRate < 100) {
    mTickRateText->setColour(
        Ogre::ColourValue(1.0f, 0.5f, 0.0f)); // Orange (fast)
  } else {
    mTickRateText->setColour(
        Ogre::ColourValue(1.0f, 0.0f, 0.0f)); // Red (very fast)
  }
}

void Overlay::updateTeamText() {
  if (!mGameManager) {
    return;
  }

  std::unordered_set<std::string> teams = mGameManager->getTeams();
  PlayerMap players = mGameManager->getPlayers();

  std::stringstream ss;
  ss << "Teams:\n";
  for (const auto& team : teams) {
    mPanel->setDimensions(300, 120 + teams.size() * 16);
    int playerCount = 0;
    for (const auto& playerPair : players) {
      if (playerPair.second->getTeamName() == team) {
        playerCount++;
      }
    }
    ss << team << " - player alive: " << playerCount << "\n";
  }
  mTeamText->setCaption(ss.str());
}

void Overlay::setStatusText(const std::string &text) {
  if (mStatusText) {
    mStatusText->setCaption(text);
  }
}

void Overlay::show() {
  if (mOverlay) {
    mOverlay->show();
  }
}

void Overlay::hide() {
  if (mOverlay) {
    mOverlay->hide();
  }
}