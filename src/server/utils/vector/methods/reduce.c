/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** reduce
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stddef.h>
#include <string.h>

void vector_reduce_impl(const vector_t *self,
    reduce_function_t reduce_fn, accumulator_t *acc, void *context)
{
    size_t start;
    void *element;

    assert(self && reduce_fn && acc && acc->result);
    if (acc->initial_value) {
        memcpy(acc->result, acc->initial_value, self->element_size);
    } else if (self->size > 0) {
        memcpy(acc->result, self->public.vtable->front(self),
            self->element_size);
    }
    start = acc->initial_value ? 0 : 1;
    for (size_t i = start; i < self->size; i++) {
        element = (char *)self->data + (i * self->element_size);
        reduce_fn(acc->result, element, context);
    }
    return;
}
