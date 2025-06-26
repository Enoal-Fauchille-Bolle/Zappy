/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Scene
*/

#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <string>
#include <map>
#include <OGRE/Ogre.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreInput.h>

class Scene : public OgreBites::InputListener {
    public:
        Scene();
        ~Scene();

        void CreateScene(std::string name);
        void Initialize(OgreBites::ApplicationContext* appContext);
        void Update(float deltaTime);
        void Render();

        Ogre::SceneNode* getSceneNode(const std::string &entityName);

        // Camera control
        void setCameraPosition(const Ogre::Vector3 &position);
        void setCameraLookAt(const Ogre::Vector3 &target);
        // Camera angle control
        void rotateCamera(float yawDegrees, float pitchDegrees = 0.0f);

        Ogre::SceneManager* getSceneManager() const;

        // Input event handlers
        bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
        bool keyReleased(const OgreBites::KeyboardEvent& evt) override;
        bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;
        bool mousePressed(const OgreBites::MouseButtonEvent& evt) override;
        bool mouseReleased(const OgreBites::MouseButtonEvent& evt) override;
        bool mouseWheelRolled(const OgreBites::MouseWheelEvent& evt) override;

    protected:
    private:
        Ogre::SceneManager* mSceneManager;
        Ogre::Camera* mCamera;
        Ogre::RenderWindow* mRenderWindow;
        OgreBites::ApplicationContext* mAppContext;
        
        std::string _name;
        std::map<std::string, Ogre::SceneNode*> _entityNodes;

        // Camera movement variables
        bool mRightMouseDown;
        bool mMiddleMouseDown;
        int mLastMouseX;
        int mLastMouseY;
        int mInitialMouseX;   // Initial position when right button is pressed
        int mInitialMouseY;   // Initial position when right button is pressed
        int mCenterX;         // Center X position for cursor reset
        int mCenterY;         // Center Y position for cursor reset
        bool mCursorGrabbed;  // Whether the cursor is currently grabbed
        float mCameraSpeed;
        
        // Keyboard state tracking
        bool mKeyPressed[4]; // [Z, Q, S, D]
        
        void setupScene();
        void setupCamera();
        void setupLighting();
        void resetCursorPosition(); // Function to reset cursor to center position when moving camera
};

#endif /* !SCENE_HPP_ */
