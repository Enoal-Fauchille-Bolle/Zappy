/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** assign
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

void vector_assign_impl(vector_t *self, size_t count, const void *value)
{
    void *dest = NULL;

    assert(self && value);
    if (count > self->capacity) {
        vector_reserve_impl(self, count);
    }
    for (size_t i = 0; i < count; i++) {
        dest = (char *)self->data + (i * self->element_size);
        memcpy(dest, value, self->element_size);
    }
    self->size = count;
    return;
}
