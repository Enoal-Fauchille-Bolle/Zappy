/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** back
*/

#include "../vector_internal.h"
#include <assert.h>

void *vector_back_impl(const vector_t *self)
{
    assert(self && self->size > 0);
    return (char *)self->data + ((self->size - 1) * self->element_size);
}
