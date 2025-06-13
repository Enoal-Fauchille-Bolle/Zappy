/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** sort
*/

#define _GNU_SOURCE

#include "../vector_internal.h"
#include <assert.h>
#include <stdlib.h>

void vector_sort_impl(vector_t *self,
    compare_function_t compare_fn, void *context)
{
    assert(self && compare_fn);
    qsort_r(self->data, self->size,
        self->element_size, compare_fn, context);
    return;
}
