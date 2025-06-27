/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** slice
*/

#include "../vector_internal.h"
#include "server/vector.h"
#include <assert.h>
#include <stddef.h>

vector_t *vector_slice_impl(const vector_t *self, size_t start, size_t end)
{
    void *element = NULL;
    vector_t *result = NULL;
    size_t slice_size;

    assert(self && start <= end && end <= self->size);
    result = vector_new(self->element_size);
    if (!result) {
        return NULL;
    }
    slice_size = end - start;
    if (slice_size == 0) {
        return result;
    }
    vector_reserve_impl(result, slice_size);
    for (size_t i = start; i < end; i++) {
        element = (char *)self->data + (i * self->element_size);
        result->public.vtable->push_back(result, element);
    }
    return result;
}
