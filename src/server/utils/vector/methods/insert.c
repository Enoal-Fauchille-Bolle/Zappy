/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** insert
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

static void move_forward(vector_t *self, size_t index)
{
    void *src = (char *)self->data + (index * self->element_size);
    void *dest = (char *)self->data + ((index + 1) * self->element_size);
    size_t move_size = (self->size - index) * self->element_size;

    memmove(dest, src, move_size);
    return;
}

void vector_insert_impl(vector_t *self, size_t index, const void *item)
{
    void *insert_pos;

    assert(self && item && index <= self->size);
    if (index < self->size) {
        move_forward(self, index);
    }
    insert_pos = (char *)self->data + (index * self->element_size);
    memcpy(insert_pos, item, self->element_size);
    self->size++;
    return;
}
