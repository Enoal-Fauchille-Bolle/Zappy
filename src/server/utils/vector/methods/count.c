/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** count
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>

size_t vector_count_impl(
    const vector_t *self, predicate_function_t predicate, void *context)
{
    size_t count = 0;
    void *element;

    assert(self && predicate);
    for (size_t i = 0; i < self->size; i++) {
        element = (char *)self->data + (i * self->element_size);
        if (predicate(element, context)) {
            count++;
        }
    }
    return count;
}
