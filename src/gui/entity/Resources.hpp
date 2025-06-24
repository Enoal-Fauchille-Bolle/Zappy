/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resources
*/

#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include "Entity.hpp"
#include "../Types.hpp"

class Resources : public Entity{
    public:
        Resources();
        Resources(const std::string &id, const std::string &meshName, ResourceType type = ResourceType::FOOD);
        ~Resources();

        void initialize() override;
        void setResourceType(ResourceType type);
        void applyResourceColor();
        ResourceType getResourceType() const;

    protected:
    private:
        ResourceType _type;
};

#endif /* !RESOURCES_HPP_ */
