/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** reserve
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void vector_reserve_impl(vector_t *self, size_t new_cap)
{
    void *new_data = NULL;

    assert(self);
    if (new_cap <= self->capacity) {
        return;
    }
    new_data = realloc(self->data, new_cap * self->element_size);
    if (!new_data) {
        perror("vector_reserve realloc failed");
        return;
    }
    self->data = new_data;
    self->capacity = new_cap;
    return;
}
