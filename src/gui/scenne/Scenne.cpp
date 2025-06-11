/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Scenne
*/

#include "Scenne.hpp"
#include <iostream>

Scenne::Scenne() : mSceneManager(nullptr), mCamera(nullptr), mRenderWindow(nullptr), 
                   mAppContext(nullptr)
{
}

Scenne::~Scenne()
{
}

void Scenne::CreateScenne(std::string name)
{
    _name = name.empty() ? "DefaultScene" : name;
}

void Scenne::Initialize(OgreBites::ApplicationContext* appContext)
{
    mAppContext = appContext;
    mRenderWindow = appContext->getRenderWindow();
    
    setupScene();
    setupCamera();
    setupLighting();
    
    // Add this scene as input listener
    appContext->addInputListener(this);
}

void Scenne::setupScene()
{
    mSceneManager = Ogre::Root::getSingleton().createSceneManager();
    
    // Set default ambient light
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
    
    // Set sky
    mSceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 100);
}

void Scenne::setupCamera()
{
    mCamera = mSceneManager->createCamera("MainCamera");
    mCamera->setNearClipDistance(5);
    mCamera->setAutoAspectRatio(true);
    
    // Create a camera node
    Ogre::SceneNode* camNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    camNode->attachObject(mCamera);
    
    // Position the camera
    camNode->setPosition(0, 50, 80);
    camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
    
    // Create viewport
    mRenderWindow->addViewport(mCamera);
}

void Scenne::setupLighting()
{
    Ogre::Light* light = mSceneManager->createLight("MainLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    Ogre::SceneNode* lightNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setDirection(Ogre::Vector3(0, -1, -1), Ogre::Node::TS_WORLD);
    light->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    light->setSpecularColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
}

void Scenne::addEntity(const std::string &meshName, const std::string &entityName)
{
    Ogre::Entity* entity = mSceneManager->createEntity(entityName, meshName);
    Ogre::SceneNode* node = mSceneManager->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    
    // Store the node for later access
    _entityNodes[entityName] = node;
}

void Scenne::moveEntity(const std::string &entityName, const Ogre::Vector3 &position)
{
    auto it = _entityNodes.find(entityName);
    if (it != _entityNodes.end()) {
        it->second->setPosition(position);
    } else {
        std::cerr << "Entity not found: " << entityName << std::endl;
    }
}

Ogre::SceneNode* Scenne::getSceneNode(const std::string &entityName)
{
    auto it = _entityNodes.find(entityName);
    if (it != _entityNodes.end()) {
        return it->second;
    }
    return nullptr;
}

void Scenne::setCameraPosition(const Ogre::Vector3 &position)
{
    mCamera->getParentSceneNode()->setPosition(position);
}

void Scenne::setCameraLookAt(const Ogre::Vector3 &target)
{
    mCamera->getParentSceneNode()->lookAt(target, Ogre::Node::TS_WORLD);
}

void Scenne::Update(float /*deltaTime*/)
{
    // Update scene entities if needed
    // This is where you would animate objects, update physics, etc.
}

void Scenne::Render()
{
    // Nothing to do here as Ogre automatically renders the scene
}

bool Scenne::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    // Handle keyboard input
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
        return false; // Allow app to close
    }
    
    // Camera movement
    Ogre::Vector3 cameraPos = mCamera->getParentSceneNode()->getPosition();
    
    if (evt.keysym.sym == 'w') {
        cameraPos.z -= 10.0f;
    } else if (evt.keysym.sym == 's') {
        cameraPos.z += 10.0f;
    } else if (evt.keysym.sym == 'a') {
        cameraPos.x -= 10.0f;
    } else if (evt.keysym.sym == 'd') {
        cameraPos.x += 10.0f;
    }
    
    mCamera->getParentSceneNode()->setPosition(cameraPos);
    
    return true;
}

bool Scenne::mouseMoved(const OgreBites::MouseMotionEvent& /*evt*/)
{
    // Handle mouse movement
    return true;
}

Ogre::SceneManager* Scenne::getSceneManager() const
{
    return mSceneManager;
}