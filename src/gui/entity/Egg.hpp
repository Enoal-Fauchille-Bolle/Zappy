/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player entity class
*/

#ifndef EGG_HPP_
#define EGG_HPP_

#include "Entity.hpp"
#include <string>
#include <map>

class Egg : public Entity {
    public:
        Egg(int id, int parentId);
        ~Egg();

        void initialize() override;

        int getParentId() const;
        int getEggId() const;

    private:
        int _eggId;
        int _parentId;

};

#endif /* !EGG_HPP_ */
