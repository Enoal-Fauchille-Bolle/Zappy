/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** capacity
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>

size_t vector_capacity_impl(const vector_t *self)
{
    assert(self);
    return self->capacity;
}
