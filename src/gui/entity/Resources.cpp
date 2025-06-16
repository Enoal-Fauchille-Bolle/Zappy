/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resources
*/

#include "Resources.hpp"

Resources::Resources()
    : Entity("resource", "knot.mesh"), _type(ResourceType::FOOD)
{
    setScale(0.5f, 0.5f, 0.5f); // Set a default scale for resources
}

Resources::Resources(const std::string &id, const std::string &meshName, ResourceType type)
    : Entity(id, meshName), _type(type)
{
    setScale(0.5f, 0.5f, 0.5f); // Set a default scale for resources
}

Resources::~Resources()
{
}

void Resources::setResourceType(ResourceType type)
{
    _type = type;
}

void Resources::applyResourceColor()
{
    switch (_type) {
        case ResourceType::FOOD:
            this->setColor({1.0f, 0.5f, 0.0f, 1.0f}); // Orange
            break;
        case ResourceType::LINEMATE:
            this->setColor({0.5f, 0.5f, 0.5f, 1.0f}); // Gray
            break;
        case ResourceType::DERAUMERE:
            this->setColor({10.0f, 0.0f, 0.0f, 1.0f}); // Red
            break;
        case ResourceType::SIBUR:
            this->setColor({0.01f, 10.0f, 0.0, 1.0f}); // Green
            break;
        case ResourceType::MENDIANE:
            this->setColor({0.2f, 0.2f, 0.8f, 1.0f}); // Blue
            break;
        case ResourceType::PHIRAS:
            this->setColor({1.0f, 1.0f, 0.2f, 1.0f}); // Yellow
            break;
        case ResourceType::THYSTAME:
            this->setColor({1.0f, 0.2f, 1.0f, 1.0f}); // Magenta
            break;
    }
}

ResourceType Resources::getResourceType() const
{
    return _type;
}

