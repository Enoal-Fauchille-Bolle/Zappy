/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** vector
*/

#include "server/vector.h"
#include "vector_internal.h"
#include <stdlib.h>

const vector_vtable_t vector_vtable = {
    .at = vector_at_impl,
    .front = vector_front_impl,
    .back = vector_back_impl,
    .data = vector_data_impl,
    .empty = vector_empty_impl,
    .size = vector_size_impl,
    .capacity = vector_capacity_impl,
    .reserve = vector_reserve_impl,
    .shrink_to_fit = vector_shrink_to_fit_impl,
    .clear = vector_clear_impl,
    .push_back = vector_push_back_impl,
    .pop_back = vector_pop_back_impl,
    .insert = vector_insert_impl,
    .erase = vector_erase_impl,
    .resize = vector_resize_impl,
    .assign = vector_assign_impl,
    .map = vector_map_impl,
    .filter = vector_filter_impl,
    .reduce = vector_reduce_impl,
    .for_each = vector_for_each_impl,
    .sort = vector_sort_impl,
    .any = vector_any_impl,
    .all = vector_all_impl,
    .count = vector_count_impl,
    .find = vector_find_impl,
    .reverse = vector_reverse_impl,
    .slice = vector_slice_impl
};

vector_t *vector_new(size_t element_size)
{
    vector_t *self = malloc(sizeof(vector_t));

    if (!self) {
        return NULL;
    }
    *self = (vector_t){
        .public.vtable = &vector_vtable,
        .data = NULL,
        .size = 0,
        .capacity = 0,
        .element_size = element_size
    };
    return self;
};

void vector_destroy(vector_t *self)
{
    if (!self) {
        return;
    }
    if (self->data) {
        free(self->data);
    }
    free(self);
    self = NULL;
    return;
}

const vector_vtable_t *vector_get_vtable(vector_t *self)
{
    return self ? self->public.vtable : NULL;
}
