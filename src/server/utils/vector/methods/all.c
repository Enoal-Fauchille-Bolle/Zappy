/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** all
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stdbool.h>

bool vector_all_impl(
    const vector_t *self, predicate_function_t predicate, void *context)
{
    void *element;

    assert(self && predicate);
    for (size_t i = 0; i < self->size; i++) {
        element = (char *)self->data + (i * self->element_size);
        if (!predicate(element, context)) {
            return false;
        }
    }
    return true;
}
