/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** at
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>

void *vector_at_impl(const vector_t *self, size_t index)
{
    assert(self && index < self->size);
    return (char *)self->data + (index * self->element_size);
}
