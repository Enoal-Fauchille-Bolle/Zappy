/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Zappy Application
*/

#ifndef ZAPPYAPP_HPP_
#define ZAPPYAPP_HPP_

#include <OGRE/Ogre.h>
#include <OGRE/Bites/OgreApplicationContext.h>
#include "scene/Scene.hpp"

class ZappyApp : public OgreBites::ApplicationContext {
    public:
        ZappyApp();
        ~ZappyApp();

        void setup() override;
        Scene& getScene();

    private:
        Scene _scene;
};

#endif /* !ZAPPYAPP_HPP_ */
