/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** shrink_to_fit
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void *shrink(vector_t *self)
{
    void *new_data = realloc(self->data, self->size * self->element_size);

    if (!new_data) {
        perror("vector_shrink_to_fit realloc failed");
        return NULL;
    }
    return new_data;
}

void vector_shrink_to_fit_impl(vector_t *self)
{
    void *new_data = NULL;

    assert(self);
    if (self->size == self->capacity) {
        return;
    }
    if (self->size == 0) {
        free(self->data);
        self->data = NULL;
        self->capacity = 0;
        return;
    }
    new_data = shrink(self);
    if (!new_data)
        return;
    self->data = new_data;
    self->capacity = self->size;
    return;
}
