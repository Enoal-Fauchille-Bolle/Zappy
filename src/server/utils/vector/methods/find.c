/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** find
*/

#include "../vector_internal.h"
#include "server/vector.h"
#include <assert.h>
#include <stddef.h>

void *vector_find_impl(
    const vector_t *self, predicate_function_t predicate, void *context)
{
    void *element;

    assert(self && predicate);
    for (size_t i = 0; i < self->size; i++) {
        element = (char *)self->data + (i * self->element_size);
        if (predicate(element, context)) {
            return element;
        }
    }
    return NULL;
}
