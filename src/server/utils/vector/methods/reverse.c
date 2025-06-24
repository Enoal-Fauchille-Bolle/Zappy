/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** reverse
*/

#include "../vector_internal.h"
#include "server/vector.h"
#include <assert.h>
#include <stddef.h>

vector_t *vector_reverse_impl(const vector_t *self)
{
    vector_t *result;
    void *element;

    assert(self);
    result = vector_new(self->element_size);
    if (!result) {
        return NULL;
    }
    vector_reserve_impl(result, self->size);
    for (size_t i = 0; i < self->size; i++) {
        element =
            (char *)self->data + ((self->size - 1 - i) * self->element_size);
        result->public.vtable->push_back(result, element);
    }
    return result;
}
