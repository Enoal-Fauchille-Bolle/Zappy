/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Entity class implementation
*/

#include "Entity.hpp"
#include <iostream>

Entity::Entity(const std::string &id, const std::string &meshName)
    : _id(id), _meshName(meshName), _ogreEntity(nullptr), _sceneNode(nullptr), _isAttached(false)
{
}

Entity::~Entity()
{
}

void Entity::setPosition(float x, float y, float z)
{
    if (_sceneNode) {
        _sceneNode->setPosition(x, y, z);
    }
}

void Entity::setRotation(float x, float y, float z)
{
    if (_sceneNode) {
        _sceneNode->resetOrientation();
        _sceneNode->pitch(Ogre::Degree(x));
        _sceneNode->yaw(Ogre::Degree(y));
        _sceneNode->roll(Ogre::Degree(z));
    }
}

void Entity::setScale(float x, float y, float z)
{
    if (_sceneNode) {
        _sceneNode->setScale(x, y, z);
    }
}

std::string Entity::getId() const
{
    return _id;
}

Ogre::SceneNode* Entity::getSceneNode() const
{
    return _sceneNode;
}

void Entity::attachToScene(Ogre::SceneManager* sceneManager)
{
    if (!_isAttached && sceneManager) {
        try {
            _ogreEntity = sceneManager->createEntity(_id, "cube.mesh");
            _sceneNode = sceneManager->getRootSceneNode()->createChildSceneNode(_id + "_node");
            _sceneNode->attachObject(_ogreEntity);
            _isAttached = true;
        } catch (const Ogre::Exception& e) {
            std::cerr << "Error attaching entity to scene: " << e.what() << std::endl;
        }
    }
}
