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
#include "../Types.hpp"

class Entity {
    public:
        Entity(const std::string &id, const std::string &meshName);
        virtual ~Entity();

        virtual void initialize(); // New: virtual initialize method

        void setPosition(const Position& pos);
        void setPosition(float x, float y, float z);
        void setRotation(float x, float y, float z);
        void setScale(float x, float y, float z);
        void setColor(const Color& color);

        std::string getId() const;
        Ogre::SceneNode* getSceneNode() const;
        void attachToScene(Ogre::SceneManager* sceneManager);
        void replaceMesh(const std::string &meshName);

    private:
        std::string _id;
        std::string _meshName;
        Ogre::Entity* _ogreEntity;
        Ogre::SceneNode* _sceneNode;
        bool _isAttached;
};

#endif /* !ENTITY_HPP_ */
