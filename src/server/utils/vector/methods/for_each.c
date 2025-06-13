/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** for_each
*/

#include "../vector_internal.h"
#include <assert.h>

void vector_for_each_impl(vector_t *self,
    for_each_function_t foreach_fn, void *context)
{
    void *element;

    assert(self && foreach_fn);
    for (size_t i = 0; i < self->size; i++) {
        element = (char *)self->data + (i *self->element_size);
        foreach_fn(element, i, context);
    }
    return;
}
