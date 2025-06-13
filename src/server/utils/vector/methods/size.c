/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** size
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>

size_t vector_size_impl(const vector_t *self)
{
    assert(self);
    return self->size;
}
