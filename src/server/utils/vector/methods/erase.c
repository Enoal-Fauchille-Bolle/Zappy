/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** erase
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

static void move_back(vector_t *self, size_t index)
{
    void *dest = (char *)self->data + (index * self->element_size);
    void *src = (char *)self->data + ((index + 1) * self->element_size);
    size_t move_size = (self->size - index - 1) * self->element_size;

    memmove(dest, src, move_size);
    return;
}

void vector_erase_impl(vector_t *self, size_t index)
{
    assert(self && index < self->size);
    if (index < self->size - 1) {
        move_back(self, index);
    }
    self->size--;
    return;
}
