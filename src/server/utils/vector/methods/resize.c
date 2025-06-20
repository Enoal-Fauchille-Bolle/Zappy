/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** resize
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

void vector_resize_impl(
    vector_t *self, size_t new_size, const void *default_value)
{
    void *dest;

    assert(self);
    if (new_size > self->capacity) {
        vector_reserve_impl(self, new_size);
    }
    if (new_size > self->size && default_value) {
        for (size_t i = self->size; i < new_size; i++) {
            dest = (char *)self->data + (i * self->element_size);
            memcpy(dest, default_value, self->element_size);
        }
    }
    self->size = new_size;
    return;
}
