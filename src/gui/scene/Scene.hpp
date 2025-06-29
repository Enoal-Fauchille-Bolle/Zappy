/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Scene
*/

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreInput.h>
#include <OGRE/Ogre.h>
#include <map>
#include <string>

class SimpleGameManager;

#include "../overlay/Overlay.hpp"

class Scene : public OgreBites::InputListener {
public:
  Scene();
  ~Scene();

  void CreateScene(std::string name);
  void Initialize(OgreBites::ApplicationContext *appContext);
  void Update(float deltaTime, int tickRate = 100);
  void Render();

  Ogre::SceneNode *getSceneNode(const std::string &entityName);

  void setCameraPosition(const Ogre::Vector3 &position);
  void setCameraLookAt(const Ogre::Vector3 &target);
  void rotateCamera(float yawDegrees, float pitchDegrees = 0.0f);

  Ogre::SceneManager *getSceneManager() const;

  void setGameManager(SimpleGameManager *gameManager);

  void frameRenderingQueued(const Ogre::FrameEvent &evt);

  bool keyPressed(const OgreBites::KeyboardEvent &evt) override;
  bool keyReleased(const OgreBites::KeyboardEvent &evt) override;
  bool mouseMoved(const OgreBites::MouseMotionEvent &evt) override;
  bool mousePressed(const OgreBites::MouseButtonEvent &evt) override;
  bool mouseReleased(const OgreBites::MouseButtonEvent &evt) override;
  bool mouseWheelRolled(const OgreBites::MouseWheelEvent &evt) override;

protected:
private:
  Ogre::SceneManager *mSceneManager;
  Ogre::Camera *mCamera;
  Ogre::RenderWindow *mRenderWindow;
  OgreBites::ApplicationContext *mAppContext;

  SimpleGameManager *mGameManager;

  Overlay mOverlay;

  std::string _name;
  std::map<std::string, Ogre::SceneNode *> _entityNodes;
  bool mRightMouseDown;
  bool mMiddleMouseDown;
  int mLastMouseX;
  int mLastMouseY;
  int mInitialMouseX;
  int mInitialMouseY;
  int mCenterX;
  int mCenterY;
  bool mCursorGrabbed;
  float mCameraSpeed;
  bool mKeyPressed[6];
  void setupScene();
  void setupCamera();
  void setupLighting();
  void setupOverlay();
  void resetCursorPosition();
};

#endif /* !SCENE_HPP_ */
