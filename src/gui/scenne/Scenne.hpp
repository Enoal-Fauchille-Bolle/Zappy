/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Scenne
*/

#ifndef SCENNE_HPP_
#define SCENNE_HPP_

#include <string>
#include <map>
#include <OGRE/Ogre.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include <OGRE/Bites/OgreInput.h>

class Scenne : public OgreBites::InputListener {
    public:
        Scenne();
        ~Scenne();

        void CreateScenne(std::string name);
        void Initialize(OgreBites::ApplicationContext* appContext);
        void Update(float deltaTime);
        void Render();
        
        // Entity management
        void addEntity(const std::string &meshName, const std::string &entityName);
        void moveEntity(const std::string &entityName, const Ogre::Vector3 &position);
        Ogre::SceneNode* getSceneNode(const std::string &entityName);

        // Camera control
        void setCameraPosition(const Ogre::Vector3 &position);
        void setCameraLookAt(const Ogre::Vector3 &target);
        
        // Scene manager access
        Ogre::SceneManager* getSceneManager() const;
        
        bool keyPressed(const OgreBites::KeyboardEvent& evt) override;
        bool mouseMoved(const OgreBites::MouseMotionEvent& evt) override;

        void createDebugGrid(int width, int height, float spacing);

    protected:
    private:
        Ogre::SceneManager* mSceneManager;
        Ogre::Camera* mCamera;
        Ogre::RenderWindow* mRenderWindow;
        OgreBites::ApplicationContext* mAppContext;
        
        std::string _name;
        std::map<std::string, Ogre::SceneNode*> _entityNodes;

        void setupScene();
        void setupCamera();
        void setupLighting();
};

#endif /* !SCENNE_HPP_ */
