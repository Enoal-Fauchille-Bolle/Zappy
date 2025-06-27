/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** map
*/

#include "../vector_internal.h"
#include "server/vector.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

static bool map_over(const vector_t *self, vector_t *result,
    map_function_t map_fn, void *context)
{
    void *input = NULL;
    void *output = NULL;

    for (size_t i = 0; i < self->size; i++) {
        input = (char *)self->data + (i * self->element_size);
        output = malloc(result->element_size);
        if (!output) {
            perror("vector_map");
            vector_destroy(result);
            return false;
        }
        map_fn(input, output, context);
        result->public.vtable->push_back(result, output);
        free(output);
    }
    return true;
}

vector_t *vector_map_impl(const vector_t *self, map_function_t map_fn,
    size_t output_element_size, void *context)
{
    vector_t *result = NULL;

    assert(self && map_fn);
    result = vector_new(output_element_size);
    if (!result) {
        return NULL;
    }
    vector_reserve_impl(result, self->size);
    if (!map_over(self, result, map_fn, context)) {
        return NULL;
    }
    return result;
}
