/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Entity class for game objects
*/

#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <string>
#include <OGRE/Ogre.h>

class Entity {
    public:
        Entity(const std::string &id, const std::string &meshName);
        virtual ~Entity();

        void setPosition(float x, float y, float z);
        void setRotation(float x, float y, float z);
        void setScale(float x, float y, float z);
        
        std::string getId() const;
        Ogre::SceneNode* getSceneNode() const;
        void attachToScene(Ogre::SceneManager* sceneManager);

    private:
        std::string _id;
        std::string _meshName;
        Ogre::Entity* _ogreEntity;
        Ogre::SceneNode* _sceneNode;
        bool _isAttached;
};

#endif /* !ENTITY_HPP_ */
