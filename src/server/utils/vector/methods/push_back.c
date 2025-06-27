/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** push_back
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

void vector_push_back_impl(vector_t *self, const void *item)
{
    void *dest = NULL;
    size_t new_cap;

    assert(self && item);
    if (self->size >= self->capacity) {
        new_cap = self->capacity == 0 ? 1 : self->capacity * 2;
        vector_reserve_impl(self, new_cap);
    }
    dest = (char *)self->data + (self->size * self->element_size);
    memcpy(dest, item, self->element_size);
    self->size++;
    return;
}
