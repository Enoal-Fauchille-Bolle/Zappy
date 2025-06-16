/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** front
*/

#include "../vector_internal.h"
#include <assert.h>

void *vector_front_impl(const vector_t *self)
{
    assert(self && self->size > 0);
    return self->data;
}
