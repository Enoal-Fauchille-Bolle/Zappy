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
    appContext->addInputListener(this);
}

void Scenne::setupScene()
{
    mSceneManager = Ogre::Root::getSingleton().createSceneManager();
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
    mSceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 100);
}

void Scenne::setupCamera()
{
    mCamera = mSceneManager->createCamera("MainCamera");
    mCamera->setNearClipDistance(5);
    mCamera->setAutoAspectRatio(true);

    Ogre::SceneNode* camNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
    camNode->attachObject(mCamera);
    // Position camera directly above the scene for a top-down view
    camNode->setPosition(0, 150, 0);
    // Make the camera look straight down
    camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
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

void Scenne::Update(float)
{
}

void Scenne::Render()
{
}

bool Scenne::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
        return false;
    }
    
    Ogre::Vector3 cameraPos = mCamera->getParentSceneNode()->getPosition();
    
    if (evt.keysym.sym == 'z') {
        cameraPos.z -= 10.0f;
    } else if (evt.keysym.sym == 's') {
        cameraPos.z += 10.0f;
    } else if (evt.keysym.sym == 'q') {
        cameraPos.x -= 10.0f;
    } else if (evt.keysym.sym == 'd') {
        cameraPos.x += 10.0f;
    } else if (evt.keysym.sym == 'e') {
        // Zoom in - move camera closer to target
        cameraPos.y = std::max(10.0f, cameraPos.y - 10.0f);
    } else if (evt.keysym.sym == 'r') {
        // Zoom out - move camera further from target
        cameraPos.y = std::min(300.0f, cameraPos.y + 10.0f);
    }
    
    mCamera->getParentSceneNode()->setPosition(cameraPos);
    return true;
}

bool Scenne::mouseMoved(const OgreBites::MouseMotionEvent&)
{
    return true;
}

Ogre::SceneManager* Scenne::getSceneManager() const
{
    return mSceneManager;
}