/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity class
*/

#include "Egg.hpp"
#include <iostream>
#include "OGRE/Ogre.h"

Egg::Egg(int id, int parentId)
    : Entity("egg_" + std::to_string(id), "sphere.mesh"),
      _eggId(id),
      _parentId(parentId)
{ 
}

Egg::~Egg()
{
    // Destructor logic if needed
    std::cout << "Egg " << _eggId << " destroyed." << std::endl;
}

int Egg::getParentId() const
{
    return _parentId;
}

int Egg::getEggId() const
{
    return _eggId;
}
