#include "Scenne.hpp"
#include <iostream>

/**
 * @brief Default constructor for the Scenne class
 */
Scenne::Scenne() :
        mSceneManager(nullptr),
        mCamera(nullptr),
        mRenderWindow(nullptr),
        mAppContext(nullptr)
{
}

/**
 * @brief Destructor for the Scenne class
 */
Scenne::~Scenne()
{
}

/**
 * @brief Creates a new scene with the specified name
 * @param name The name of the scene, defaults to "DefaultScene" if empty
 */
void Scenne::CreateScenne(std::string name)
{
    _name = name.empty() ? "DefaultScene" : name;
}

/**
 * @brief Initializes the scene with required components
 * @param appContext The Ogre application context
 */
void Scenne::Initialize(OgreBites::ApplicationContext* appContext)
{
    mAppContext = appContext;
    mRenderWindow = appContext->getRenderWindow();
    setupScene();
    setupCamera();
    setupLighting();
    appContext->addInputListener(this);
}

/**
 * @brief Sets up the scene manager and environment
 */
void Scenne::setupScene()
{
    mSceneManager = Ogre::Root::getSingleton().createSceneManager();
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
    mSceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 100);
}

/**
 * @brief Sets up the main camera for the scene
 */
void Scenne::setupCamera()
{
    mCamera = mSceneManager->createCamera("MainCamera");
    mCamera->setNearClipDistance(5);
    mCamera->setAutoAspectRatio(true);

    Ogre::SceneNode* camNode =
        mSceneManager->getRootSceneNode()->createChildSceneNode();
    camNode->attachObject(mCamera);
    camNode->setPosition(0, 150, 0);
    camNode->lookAt(Ogre::Vector3(0, 0, 0), Ogre::Node::TS_WORLD);
    mRenderWindow->addViewport(mCamera);
}

/**
 * @brief Sets up the lighting for the scene
 */
void Scenne::setupLighting()
{
    Ogre::Light* light = mSceneManager->createLight("MainLight");
    light->setType(Ogre::Light::LT_DIRECTIONAL);
    Ogre::SceneNode* lightNode =
        mSceneManager->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(light);
    lightNode->setDirection(Ogre::Vector3(0, -1, -1), Ogre::Node::TS_WORLD);
    light->setDiffuseColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
    light->setSpecularColour(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
}

/**
 * @brief Adds an entity to the scene
 * @param meshName The name of the mesh resource
 * @param entityName The name for the entity instance
 */
void Scenne::addEntity(const std::string& meshName,
                       const std::string& entityName)
{
    Ogre::Entity* entity = mSceneManager->createEntity(entityName, meshName);
    Ogre::SceneNode* node =
        mSceneManager->getRootSceneNode()->createChildSceneNode();
    node->attachObject(entity);
    _entityNodes[entityName] = node;
}

/**
 * @brief Moves an entity to a new position
 * @param entityName The name of the entity to move
 * @param position The new position vector
 */
void Scenne::moveEntity(const std::string& entityName,
                        const Ogre::Vector3& position)
{
    auto it = _entityNodes.find(entityName);
    if (it != _entityNodes.end()) {
        it->second->setPosition(position);
    } else {
        std::cerr << "Entity not found: " << entityName << std::endl;
    }
}

/**
 * @brief Gets the scene node for a specific entity
 * @param entityName The name of the entity
 * @return The scene node pointer, or nullptr if not found
 */
Ogre::SceneNode* Scenne::getSceneNode(const std::string& entityName)
{
    auto it = _entityNodes.find(entityName);
    if (it != _entityNodes.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * @brief Sets the camera's position
 * @param position The new position vector for the camera
 */
void Scenne::setCameraPosition(const Ogre::Vector3& position)
{
    mCamera->getParentSceneNode()->setPosition(position);
}

/**
 * @brief Points the camera at a target location
 * @param target The target position vector to look at
 */
void Scenne::setCameraLookAt(const Ogre::Vector3& target)
{
    mCamera->getParentSceneNode()->lookAt(target, Ogre::Node::TS_WORLD);
}

/**
 * @brief Updates the scene
 * @param deltaTime The time elapsed since the last update
 */
void Scenne::Update(float)
{
}

/**
 * @brief Renders the scene
 */
void Scenne::Render()
{
}

/**
 * @brief Rotates the camera by specified angles
 * @param yawDegrees Rotation around the Y axis in degrees
 * @param pitchDegrees Rotation around the X axis in degrees
 */
void Scenne::rotateCamera(float yawDegrees, float pitchDegrees)
{
    Ogre::SceneNode* camNode = mCamera->getParentSceneNode();
    if (!camNode) return;
    camNode->yaw(Ogre::Degree(yawDegrees));
    if (pitchDegrees != 0.0f)
        camNode->pitch(Ogre::Degree(pitchDegrees));
}

/**
 * @brief Handles keyboard input events
 * @param evt The keyboard event
 * @return true to continue processing, false to cancel
 */
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
        cameraPos.y = std::max(10.0f, cameraPos.y - 10.0f);
    } else if (evt.keysym.sym == 'a') {
        cameraPos.y = std::min(300.0f, cameraPos.y + 10.0f);
    } else if (evt.keysym.sym == 'm') {
        rotateCamera(-10.0f, 0.0f);
    } else if (evt.keysym.sym == 'k') {
        rotateCamera(10.0f, 0.0f);
    } else if (evt.keysym.sym == 'l') {
        rotateCamera(0.0f, -5.0f);
    } else if (evt.keysym.sym == 'o') {
        rotateCamera(0.0f, 5.0f);
    }

    mCamera->getParentSceneNode()->setPosition(cameraPos);
    return true;
}

/**
 * @brief Handles mouse movement events
 * @param evt The mouse motion event
 * @return true to continue processing
 */
bool Scenne::mouseMoved(const OgreBites::MouseMotionEvent&)
{
    return true;
}

/**
 * @brief Gets the scene manager
 * @return Pointer to the scene manager
 */
Ogre::SceneManager* Scenne::getSceneManager() const
{
    return mSceneManager;
}