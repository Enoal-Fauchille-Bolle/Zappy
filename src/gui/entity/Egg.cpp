/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity class
*/

#include "Egg.hpp"
#include <iostream>
#include "OGRE/Ogre.h"

/**
 * @brief Constructor for Egg.
 * 
 * @param id Unique identifier for the egg.
 * @param parentId Identifier of the parent player.
 */
Egg::Egg(int id, int parentId)
    : Entity("egg_" + std::to_string(id), "sphere.mesh"),
      _eggId(id),
      _parentId(parentId)
{
}

/**
 * @brief Destructor for Egg.
 */
Egg::~Egg()
{
}

/**
 * @brief Get the parent player's ID.
 * 
 * @return The ID of the parent player.
 */
int Egg::getParentId() const
{
    return _parentId;
}

/**
 * @brief Get the egg's ID.
 * 
 * @return The egg's unique identifier.
 */
int Egg::getEggId() const
{
    return _eggId;
}

/**
 * @brief Initialize the egg after being attached to the scene.
 */
void Egg::initialize() {
}
