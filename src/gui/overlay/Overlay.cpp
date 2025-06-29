/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Overlay
*/

#include "Overlay.hpp"
#include "../GameManager.hpp"
#include "../Types.hpp"
#include <iostream>
#include <sstream>

const float Overlay::UPDATE_INTERVAL = 0.5f;

Overlay::Overlay()
    : mOverlay(nullptr), mPanel(nullptr), mStatusText(nullptr),
      mTickRateText(nullptr), mFoodText(nullptr), mLinemate(nullptr),
      mDeraumere(nullptr), mSibur(nullptr), mMendiane(nullptr),
      mPhiras(nullptr), mThystame(nullptr), mGameManager(nullptr) {}

Overlay::~Overlay() {}

Ogre::TextAreaOverlayElement *
Overlay::createTextElement(Ogre::OverlayManager *manager,
                           const std::string &name, int yPosition) {
  Ogre::TextAreaOverlayElement *element =
      static_cast<Ogre::TextAreaOverlayElement *>(
          manager->createOverlayElement("TextArea", name));
  element->setMetricsMode(Ogre::GMM_PIXELS);
  element->setPosition(20, yPosition);
  element->setDimensions(300, 400);
  element->setCharHeight(16);
  element->setFontName("SdkTrays/Value");
  element->setColour(Ogre::ColourValue::White);
  return element;
}

void Overlay::setGameManager(SimpleGameManager *gameManager) {
  mGameManager = gameManager;
  if (mGameManager) {
    updateTickRate();
    updateResourceCounts();
    updateTeams();
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
      mPanel->setDimensions(300, 400);

      std::cout << "Setting material for panel..." << std::endl;
      Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create(
          "GameOverlayMaterial",
          Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

      if (material) {
        material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
        material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
        material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
        material->getTechnique(0)->getPass(0)->setSceneBlending(
            Ogre::SBT_TRANSPARENT_ALPHA);
        mPanel->setMaterialName(material->getName());
      } else {
        mPanel->setMaterialName("BaseWhite");
        mPanel->setColour(Ogre::ColourValue(0.3f, 0.3f, 0.3f, 0.7f));
      }

      try {
        std::cout << "Creating text areas..." << std::endl;

        // Status text
        mStatusText = createTextElement(overlayManager, "StatusText", 20);
        mStatusText->setCaption("Zappy Game Status");
        mStatusText->setCharHeight(16);
        mStatusText->setFontName("SdkTrays/Value");
        mStatusText->setColour(Ogre::ColourValue::Black);
        mPanel->addChild(mStatusText);

        // Tick Rate text
        mTickRateText = createTextElement(overlayManager, "TickRateText", 60);
        mTickRateText->setCaption("Tick Rate: ---");
        mPanel->addChild(mTickRateText);
        mTeamsText = static_cast<Ogre::TextAreaOverlayElement *>(
            overlayManager->createOverlayElement("TextArea", "TeamText"));
        mTeamsText->setMetricsMode(Ogre::GMM_PIXELS);
        mTeamsText->setPosition(20, 100);
        mTeamsText->setCharHeight(16);
        mTeamsText->setFontName("SdkTrays/Value");
        mTeamsText->setColour(Ogre::ColourValue::Black);
        mPanel->addChild(mTeamsText);

        // Resource texts
        mFoodText = createTextElement(overlayManager, "FoodText", 100);
        mFoodText->setCaption("Food: ---");
        mFoodText->setColour(Ogre::ColourValue(0.0f, 0.8f, 0.0f));
        mPanel->addChild(mFoodText);

        mLinemate = createTextElement(overlayManager, "LinemateText", 130);
        mLinemate->setCaption("Linemate: ---");
        mLinemate->setColour(Ogre::ColourValue(0.5f, 0.5f, 0.5f)); // Gray
        mPanel->addChild(mLinemate);

        mDeraumere = createTextElement(overlayManager, "DeraumereText", 160);
        mDeraumere->setCaption("Deraumere: ---");
        mDeraumere->setColour(Ogre::ColourValue(0.0f, 0.5f, 1.0f)); // Blue
        mPanel->addChild(mDeraumere);

        mSibur = createTextElement(overlayManager, "SiburText", 190);
        mSibur->setCaption("Sibur: ---");
        mSibur->setColour(Ogre::ColourValue(0.8f, 0.8f, 0.0f)); // Yellow
        mPanel->addChild(mSibur);

        mMendiane = createTextElement(overlayManager, "MendianeText", 220);
        mMendiane->setCaption("Mendiane: ---");
        mMendiane->setColour(Ogre::ColourValue(1.0f, 0.5f, 0.0f)); // Orange
        mPanel->addChild(mMendiane);

        mPhiras = createTextElement(overlayManager, "PhirasText", 250);
        mPhiras->setCaption("Phiras: ---");
        mPhiras->setColour(Ogre::ColourValue(1.0f, 0.0f, 1.0f)); // Magenta
        mPanel->addChild(mPhiras);

        mThystame = createTextElement(overlayManager, "ThystameText", 280);
        mThystame->setCaption("Thystame: ---");
        mThystame->setColour(Ogre::ColourValue(1.0f, 0.0f, 0.0f)); // Red
        mPanel->addChild(mThystame);

        // Teams text
        mTeamsText = createTextElement(overlayManager, "TeamsText", 310);
        mTeamsText->setCaption("Teams: ---");
        mTeamsText->setColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f)); // White
        mPanel->addChild(mTeamsText);

        std::cout << "Text areas created successfully" << std::endl;
      } catch (const Ogre::Exception &e) {
        std::cerr << "Failed to create text areas: " << e.what() << std::endl;
      }

      mOverlay->add2D(mPanel);
      std::cout << "Showing overlay..." << std::endl;
      mOverlay->show();
    } catch (const Ogre::Exception &e) {
      std::cerr << "Ogre Exception during overlay creation: " << e.what()
                << std::endl;
    } catch (const std::exception &e) {
      std::cerr << "Standard Exception during overlay creation: " << e.what()
                << std::endl;
    }

    std::cout << "Overlay setup complete" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Exception in initialize: " << e.what() << std::endl;
  }
}

void Overlay::updateTickRate() {
  static int warningCount = 0;
  if (!mGameManager) {
    if (warningCount % 100 == 0) {
      std::cerr << "Warning: GameManager is null in updateTickRate()"
                << std::endl;
    }
    warningCount++;
    return;
  }
  warningCount = 0;
  int tickRate = mGameManager->getTickRate();
  std::stringstream ss;
  ss << "Tick Rate: " << tickRate;
  mTickRateText->setCaption(ss.str());
  if (tickRate < 10) {
    mTickRateText->setColour(Ogre::ColourValue(0.0f, 0.7f, 0.0f));
  } else if (tickRate < 50) {
    mTickRateText->setColour(Ogre::ColourValue(0.0f, 1.0f, 0.0f));
  } else if (tickRate < 100) {
    mTickRateText->setColour(Ogre::ColourValue(1.0f, 1.0f, 0.0f));
    mTickRateText->setColour(
        Ogre::ColourValue(1.0f, 0.5f, 0.0f)); // Orange (fast)
  } else {
    mTickRateText->setColour(Ogre::ColourValue(1.0f, 0.0f, 0.0f));
  }
}

void Overlay::updateResourceCounts() {
  if (!mGameManager) {
    return;
  }

  if (mFoodText)
    mFoodText->setCaption("Food: " +
                          std::to_string(mGameManager->getFoodCount()));

  if (mLinemate)
    mLinemate->setCaption("Linemate: " +
                          std::to_string(mGameManager->getLinemateCount()));

  if (mDeraumere)
    mDeraumere->setCaption("Deraumere: " +
                           std::to_string(mGameManager->getDeraumereCount()));

  if (mSibur)
    mSibur->setCaption("Sibur: " +
                       std::to_string(mGameManager->getSiburCount()));

  if (mMendiane)
    mMendiane->setCaption("Mendiane: " +
                          std::to_string(mGameManager->getMendianeCount()));

  if (mPhiras)
    mPhiras->setCaption("Phiras: " +
                        std::to_string(mGameManager->getPhirasCount()));

  if (mThystame)
    mThystame->setCaption("Thystame: " +
                          std::to_string(mGameManager->getThystameCount()));
}

void Overlay::updateTeamText() {
  if (!mGameManager) {
    return;
  }

  std::unordered_set<std::string> teams = mGameManager->getTeams();
  PlayerMap players = mGameManager->getPlayers();

  std::stringstream ss;
  ss << "Teams:\n";
  for (const auto &team : teams) {
    int playerCount = 0;
    for (const auto &playerPair : players) {
      if (playerPair.second->getTeamName() == team) {
        playerCount++;
      }
    }
    ss << team << " - player alive: " << playerCount << "\n";
  }
  mTeamsText->setCaption(ss.str());
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

void Overlay::updateTeams() {
  if (!mGameManager || !mTeamsText)
    return;

  // Get team names from the GameManager
  std::string teams = "Teams: ";
  const auto &teamSet = mGameManager->getTeamNames();
  if (teamSet.empty()) {
    teams += "---";
  } else {
    bool first = true;
    for (const auto &team : teamSet) {
      if (!first)
        teams += ", ";
      teams += team;
      first = false;
    }
  }
  mTeamsText->setCaption(teams);
}