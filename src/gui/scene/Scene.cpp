/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Scene
*/


#include "Scene.hpp"
#include <iostream>

/**
 * @brief Default constructor for the Scene class
 * 
 * Initializes all member variables to their default values including
 * camera settings, mouse states, and keyboard input tracking arrays.
 */
Scene::Scene() :
        mSceneManager(nullptr),
        mCamera(nullptr),
        mRenderWindow(nullptr),
        mAppContext(nullptr),
        mRightMouseDown(false),
        mMiddleMouseDown(false),
        mLastMouseX(0),
        mLastMouseY(0),
        mInitialMouseX(0),
        mInitialMouseY(0),
        mCenterX(0),
        mCenterY(0),
        mCursorGrabbed(false),
        mCameraSpeed(100.0f)
{
    for (int i = 0; i < 4; i++) {
        mKeyPressed[i] = false;
    }
}

/**
 * @brief Destructor for the Scene class
 * 
 * Cleans up allocated resources and scene objects.
 */
Scene::~Scene()
{
}

/**
 * @brief Creates a new scene with the specified name
 * 
 * @param name The name of the scene, defaults to "DefaultScene" if empty
 */
void Scene::CreateScene(std::string name)
{
    _name = name.empty() ? "DefaultScene" : name;
}

/**
 * @brief Initializes the scene with required components
 * 
 * Sets up the scene manager, camera, lighting, and registers
 * this class as an input listener with the application context.
 * 
 * @param appContext The Ogre application context for rendering
 */
void Scene::Initialize(OgreBites::ApplicationContext* appContext)
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
 * 
 * Creates the scene manager, sets ambient lighting and skybox.
 */
void Scene::setupScene()
{
    mSceneManager = Ogre::Root::getSingleton().createSceneManager();
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.2f, 0.2f, 0.2f));
    mSceneManager->setSkyBox(true, "Examples/SpaceSkyBox", 100);
}

/**
 * @brief Sets up the main camera for the scene
 * 
 * Creates and configures the camera, attaches it to a scene node,
 * and sets up the viewport.
 */
void Scene::setupCamera()
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
 * 
 * Creates and configures directional lighting for proper scene illumination.
 */
void Scene::setupLighting()
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
 * @brief Gets the scene node for a specific entity
 * 
 * @param entityName The name of the entity to find
 * @return The scene node pointer, or nullptr if not found
 */
Ogre::SceneNode* Scene::getSceneNode(const std::string& entityName)
{
    auto it = _entityNodes.find(entityName);
    if (it != _entityNodes.end()) {
        return it->second;
    }
    return nullptr;
}

/**
 * @brief Sets the camera's position in 3D space
 * 
 * @param position The new position vector for the camera
 */
void Scene::setCameraPosition(const Ogre::Vector3& position)
{
    mCamera->getParentSceneNode()->setPosition(position);
}

/**
 * @brief Points the camera at a target location
 * 
 * @param target The target position vector to look at
 */
void Scene::setCameraLookAt(const Ogre::Vector3& target)
{
    mCamera->getParentSceneNode()->lookAt(target, Ogre::Node::TS_WORLD);
}

/**
 * @brief Updates the scene state
 * 
 * Processes continuous keyboard input for camera movement and updates
 * camera position based on user input. Handles ZQSD movement relative
 * to the camera's current orientation.
 * 
 * @param deltaTime The time elapsed since the last update in seconds
 */
void Scene::Update(float deltaTime)
{
    Ogre::Vector3 cameraPos = mCamera->getParentSceneNode()->getPosition();
    Ogre::Quaternion orientation = mCamera->getParentSceneNode()->getOrientation();
    Ogre::Vector3 forward = orientation * Ogre::Vector3::NEGATIVE_UNIT_Z;
    Ogre::Vector3 right = orientation * Ogre::Vector3::UNIT_X;

    forward.y = 0;
    forward.normalise();
    right.y = 0;
    right.normalise();

    float speed = mCameraSpeed * deltaTime;

    if (mKeyPressed[0])
        cameraPos += forward * speed;
    if (mKeyPressed[2])
        cameraPos -= forward * speed;
    if (mKeyPressed[1])
        cameraPos -= right * speed;
    if (mKeyPressed[3])
        cameraPos += right * speed;

    mCamera->getParentSceneNode()->setPosition(cameraPos);
}

/**
 * @brief Renders the scene
 * 
 * Currently a placeholder for custom rendering operations.
 */
void Scene::Render()
{
}

/**
 * @brief Rotates the camera by specified angles
 * 
 * @param yawDegrees Rotation around the Y axis in degrees (horizontal)
 * @param pitchDegrees Rotation around the X axis in degrees (vertical)
 */
void Scene::rotateCamera(float yawDegrees, float pitchDegrees)
{
    Ogre::SceneNode* camNode = mCamera->getParentSceneNode();
    if (!camNode) return;
    camNode->yaw(Ogre::Degree(yawDegrees));
    if (pitchDegrees != 0.0f)
        camNode->pitch(Ogre::Degree(pitchDegrees));
}

/**
 * @brief Handles keyboard key press events
 * 
 * Processes ZQSD movement keys and escape key for application exit.
 * Sets movement flags for continuous movement processing in Update().
 * 
 * @param evt The keyboard event containing key information
 * @return true to continue processing, false to cancel/exit
 */
bool Scene::keyPressed(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
        return false;
    }

    if (evt.keysym.sym == 'z') {
        mKeyPressed[0] = true;
    } else if (evt.keysym.sym == 'q') {
        mKeyPressed[1] = true;
    } else if (evt.keysym.sym == 's') {
        mKeyPressed[2] = true;
    } else if (evt.keysym.sym == 'd') {
        mKeyPressed[3] = true;
    }

    return true;
}

/**
 * @brief Handles keyboard key release events
 * 
 * Stops movement when ZQSD keys are released.
 * 
 * @param evt The keyboard event containing key information
 * @return true to continue processing
 */
bool Scene::keyReleased(const OgreBites::KeyboardEvent& evt)
{
    if (evt.keysym.sym == 'z') {
        mKeyPressed[0] = false;
    } else if (evt.keysym.sym == 'q') {
        mKeyPressed[1] = false;
    } else if (evt.keysym.sym == 's') {
        mKeyPressed[2] = false;
    } else if (evt.keysym.sym == 'd') {
        mKeyPressed[3] = false;
    }

    return true;
}

/**
 * @brief Handles mouse movement events
 * 
 * Processes camera rotation when middle mouse button is held,
 * and camera panning when right mouse button is held.
 * 
 * @param evt The mouse motion event containing position delta
 * @return true to continue processing
 */
bool Scene::mouseMoved(const OgreBites::MouseMotionEvent& evt)
{
    if (mMiddleMouseDown) {
        int dx = evt.x - mLastMouseX;
        int dy = evt.y - mLastMouseY;

        float yawAngle = -dx * 0.2f;
        float pitchAngle = -dy * 0.2f;

        rotateCamera(yawAngle, pitchAngle);
    }

    if (mRightMouseDown) {
        int dx = evt.x - mLastMouseX;
        int dy = evt.y - mLastMouseY;

        Ogre::Vector3 cameraPos = mCamera->getParentSceneNode()->getPosition();
        Ogre::Quaternion orientation = mCamera->getParentSceneNode()->getOrientation();
        Ogre::Vector3 right = orientation * Ogre::Vector3::UNIT_X;
        Ogre::Vector3 up = orientation * Ogre::Vector3::UNIT_Y;

        cameraPos -= right * dx * 0.3f;
        cameraPos += up * dy * 0.3f;

        mCamera->getParentSceneNode()->setPosition(cameraPos);
    }

    mLastMouseX = evt.x;
    mLastMouseY = evt.y;

    return true;
}

/**
 * @brief Handles mouse button press events
 * 
 * Tracks right and middle mouse button states for camera control.
 * 
 * @param evt The mouse button event
 * @return true to continue processing
 */
bool Scene::mousePressed(const OgreBites::MouseButtonEvent& evt)
{
    if (evt.button == OgreBites::BUTTON_RIGHT) {
        mRightMouseDown = true;
    } else if (evt.button == OgreBites::BUTTON_MIDDLE) {
        mMiddleMouseDown = true;
    }

    mLastMouseX = evt.x;
    mLastMouseY = evt.y;

    return true;
}

/**
 * @brief Handles mouse button release events
 * 
 * Stops camera control when mouse buttons are released.
 * 
 * @param evt The mouse button event
 * @return true to continue processing
 */
bool Scene::mouseReleased(const OgreBites::MouseButtonEvent& evt)
{
    if (evt.button == OgreBites::BUTTON_RIGHT) {
        mRightMouseDown = false;
    } else if (evt.button == OgreBites::BUTTON_MIDDLE) {
        mMiddleMouseDown = false;
    }

    return true;
}

/**
 * @brief Handles mouse wheel scroll events
 * 
 * Provides zoom functionality by adjusting camera field of view.
 * 
 * @param evt The mouse wheel event containing scroll direction
 * @return true to continue processing
 */
bool Scene::mouseWheelRolled(const OgreBites::MouseWheelEvent& evt)
{
    Ogre::Real currentFOV = mCamera->getFOVy().valueDegrees();
    Ogre::Real zoomAmount = evt.y * 5.0f;

    Ogre::Real newFOV = currentFOV - zoomAmount;
    newFOV = std::max(10.0f, std::min(120.0f, newFOV));

    mCamera->setFOVy(Ogre::Degree(newFOV));

    return true;
}

/**
 * @brief Gets the scene manager instance
 * 
 * @return Pointer to the Ogre scene manager
 */
Ogre::SceneManager* Scene::getSceneManager() const
{
    return mSceneManager;
}