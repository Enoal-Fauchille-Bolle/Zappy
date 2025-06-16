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
#include "scenne/Scenne.hpp"

class ZappyApp : public OgreBites::ApplicationContext {
    public:
        ZappyApp();
        ~ZappyApp();

        void setup() override;
        Scenne& getScene();

    private:
        Scenne _scene;
};

#endif /* !ZAPPYAPP_HPP_ */
