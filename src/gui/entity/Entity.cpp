/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Entity class implementation
*/

#include "Entity.hpp"
#include <iostream>
#include <cstdlib>

/**
 * @brief Construct a new Entity object.
 *
 * @param id The unique identifier for the entity.
 * @param meshName The name of the mesh to use for the entity.
 */
Entity::Entity(const std::string &id, const std::string &meshName)
    : _id(id), _meshName(meshName), _ogreEntity(nullptr), _sceneNode(nullptr), _isAttached(false)
{
}

/**
 * @brief Destroy the Entity object.
 */
Entity::~Entity()
{
    Ogre::SceneManager* sceneManager = _sceneNode->getCreator();
    sceneManager->destroyEntity(_ogreEntity);
    sceneManager->destroySceneNode(_sceneNode);
}

/**
 * @brief Set the position of the entity.
 *
 * @param pos Position struct.
 */
void Entity::setPosition(const Position& pos)
{
    setPosition(pos.x, pos.y, pos.z);
}

/**
 * @brief Set the position of the entity.
 *
 * @param x X coordinate.
 * @param y Y coordinate.
 * @param z Z coordinate.
 */
void Entity::setPosition(float x, float y, float z)
{
    if (_sceneNode) {
        _sceneNode->setPosition(x, y, z);
    }
}

/**
 * @brief Set the rotation of the entity.
 *
 * @param x Pitch angle in degrees.
 * @param y Yaw angle in degrees.
 * @param z Roll angle in degrees.
 */
void Entity::setRotation(float x, float y, float z)
{
    if (_sceneNode) {
        _sceneNode->resetOrientation();
        _sceneNode->pitch(Ogre::Degree(x));
        _sceneNode->yaw(Ogre::Degree(y));
        _sceneNode->roll(Ogre::Degree(z));
    }
}

/**
 * @brief Set the scale of the entity.
 *
 * @param x Scale in X direction.
 * @param y Scale in Y direction.
 * @param z Scale in Z direction.
 */
void Entity::setScale(float x, float y, float z)
{
    if (_sceneNode) {
        _sceneNode->setScale(x, y, z);
    }
}

/**
 * @brief Set the color of the entity.
 *
 * @param color The color to set.
 */
void Entity::setColor(const Color& color)
{
    if (_ogreEntity && _ogreEntity->getNumSubEntities() > 0) {
        try {
            Ogre::SubEntity* subEntity = _ogreEntity->getSubEntity(0);
            Ogre::MaterialPtr originalMaterial = subEntity->getMaterial();
            std::string materialName = _id + "_material_" + std::to_string(rand());
            Ogre::MaterialPtr clonedMaterial = originalMaterial->clone(materialName);
            clonedMaterial->setAmbient(color.r, color.g, color.b);
            clonedMaterial->setDiffuse(color.r, color.g, color.b, color.a);
            clonedMaterial->setSpecular(color.r * 0.8f, color.g * 0.8f, color.b * 0.8f, color.a);
            clonedMaterial->setLightingEnabled(true);
            subEntity->setMaterial(clonedMaterial);
        } catch (const Ogre::Exception& e) {
            std::cerr << "Error setting entity color: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Get the ID of the entity.
 *
 * @return std::string The entity ID.
 */
std::string Entity::getId() const
{
    return _id;
}

/**
 * @brief Get the Ogre entity associated with this Entity.
 *
 * @return Ogre::Entity* Pointer to the Ogre entity.
 */
Ogre::Entity* Entity::getOgreEntity() const
{
    return _ogreEntity;
}

/**
 * @brief Get the scene node associated with the entity.
 *
 * @return Ogre::SceneNode* The scene node pointer.
 */
Ogre::SceneNode* Entity::getSceneNode() const
{
    return _sceneNode;
}

/**
 * @brief Attach the entity to the scene.
 *
 * @param sceneManager The scene manager to use.
 */
void Entity::attachToScene(Ogre::SceneManager* sceneManager)
{
    if (!_isAttached && sceneManager) {
        try {
            _ogreEntity = sceneManager->createEntity(_id, _meshName);
            _sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(_id + "_node");
            _sceneNode->attachObject(_ogreEntity);
            _isAttached = true;
        } catch (const Ogre::Exception& e) {
            std::cerr << "Error attaching entity to scene: " << e.what() << std::endl;
        }
    }
}

/**
 * @brief Initialize the entity.
 *
 * Default implementation does nothing. Derived classes can override this method
 * to perform custom initialization.
 */
void Entity::initialize()
{
    // Default: do nothing. Derived classes can override.
}

/**
 * @brief Replace the mesh of the entity.
 *
 * @param newMeshName The name of the new mesh to use.
 */
void Entity::replaceMesh(const std::string &newMeshName)
{
    Ogre::SceneManager* sceneManager = _sceneNode->getCreator();

    if (_ogreEntity) {
        _meshName = newMeshName;
        sceneManager->destroyEntity(_ogreEntity);
        _ogreEntity = sceneManager->createEntity(_id, newMeshName);
        _sceneNode->attachObject(_ogreEntity);
    }
}
