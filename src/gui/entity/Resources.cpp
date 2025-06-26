/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resources
*/

#include "Resources.hpp"

/**
 * @brief Default constructor for Resources.
 * 
 * Creates a resource with default FOOD type and sets default scale.
 */
Resources::Resources()
    : Entity("resource", "knot.mesh"), _type(ResourceType::FOOD)
{
    setScale(0.5f, 0.5f, 0.5f);
}

/**
 * @brief Parameterized constructor for Resources.
 * 
 * @param id Unique identifier for the resource.
 * @param meshName Name of the mesh file to use for rendering.
 * @param type Type of resource (default: FOOD).
 */
Resources::Resources(const std::string &id, const std::string &meshName, ResourceType type)
    : Entity(id, meshName), _type(type)
{
    setScale(0.5f, 0.5f, 0.5f);
}

/**
 * @brief Destructor for Resources.
 */
Resources::~Resources()
{
}

/**
 * @brief Set the type of this resource.
 * 
 * @param type The new resource type.
 */
void Resources::setResourceType(ResourceType type)
{
    _type = type;
    applyResourceColor();
}

/**
 * @brief Apply color to the resource based on its type.
 */
void Resources::applyResourceColor()
{
    if (!getOgreEntity()) {
        return;
    }
    switch (_type) {
        case ResourceType::FOOD:
            this->setColor(Color(1.0f, 1.0f, 0.0f, 1.0f)); // Pure Yellow
            break;
        case ResourceType::LINEMATE:
            this->setColor(Color(0.5f, 0.5f, 0.5f, 1.0f)); // Pure Gray
            break;
        case ResourceType::DERAUMERE:
            this->setColor(Color(0.0f, 1.0f, 0.0f, 1.0f)); // Pure Green
            break;
        case ResourceType::SIBUR:
            this->setColor(Color(1.0f, 0.0f, 0.0f, 1.0f)); // Pure Red
            break;
        case ResourceType::MENDIANE:
            this->setColor(Color(1.0f, 1.0f, 1.0f, 1.0f)); // Pure White
            break;
        case ResourceType::PHIRAS:
            this->setColor(Color(0.0f, 0.0f, 1.0f, 1.0f)); // Pure Blue
            break;
        case ResourceType::THYSTAME:
            this->setColor(Color(1.0f, 0.0f, 1.0f, 1.0f)); // Pure Magenta
            break;
    }
}

/**
 * @brief Get the type of this resource.
 * 
 * @return The resource type.
 */
ResourceType Resources::getResourceType() const
{
    return _type;
}

/**
 * @brief Initialize the resource after being attached to the scene.
 * 
 * Sets the proper scale and applies the appropriate color based on type.
 */
void Resources::initialize()
{
    setScale(0.01f, 0.01f, 0.01f);
    applyResourceColor();
}

