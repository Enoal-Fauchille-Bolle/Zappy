/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Entity class implementation
*/

#include "Entity.hpp"
#include <iostream>

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
void Entity::setColor(zappy_color color)
{
    if (_ogreEntity) {
        Ogre::MaterialPtr originalMaterial = _ogreEntity->getSubEntity(0)->getMaterial();
        Ogre::MaterialPtr clonedMaterial = originalMaterial->clone(_id + "_material");
        clonedMaterial->setAmbient(color.r, color.g, color.b);
        clonedMaterial->setDiffuse(color.r, color.g, color.b, color.a);
        _ogreEntity->getSubEntity(0)->setMaterial(clonedMaterial);
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
