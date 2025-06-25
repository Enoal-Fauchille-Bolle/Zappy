/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** filter
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>

vector_t *vector_filter_impl(
    const vector_t *self, predicate_function_t predicate, void *context)
{
    vector_t *result = NULL;
    void *element = NULL;

    assert(self && predicate);
    result = vector_new(self->element_size);
    if (!result) {
        return NULL;
    }
    for (size_t i = 0; i < self->size; i++) {
        element = (char *)self->data + (i * self->element_size);
        if (predicate(element, context)) {
            result->public.vtable->push_back(result, element);
        }
    }
    return result;
}
