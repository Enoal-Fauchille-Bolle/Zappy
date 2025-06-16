/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** vector_internal
*/

#ifndef VECTOR_INTERNAL_H_
    #define VECTOR_INTERNAL_H_

    #include "server/vector.h"
    #include <stddef.h>

    #define EXIT_ERROR_CODE 84

struct vector_s {
    vector_public_t public;
    vector_item_t *data;
    size_t size;
    size_t capacity;
    size_t element_size;
};

void *vector_at_impl(const vector_t *self, size_t index);
void *vector_front_impl(const vector_t *self);
void *vector_back_impl(const vector_t *self);
void *vector_data_impl(const vector_t *self);
bool vector_empty_impl(const vector_t *self);
size_t vector_size_impl(const vector_t *self);
size_t vector_capacity_impl(const vector_t *self);
void vector_reserve_impl(vector_t *self, size_t new_cap);
void vector_shrink_to_fit_impl(vector_t *self);
void vector_clear_impl(vector_t *self);
void vector_push_back_impl(vector_t *self, const void *item);
void vector_pop_back_impl(vector_t *self);
void vector_insert_impl(vector_t *self, size_t index, const void *item);
void vector_erase_impl(vector_t *self, size_t index);
void vector_resize_impl(vector_t *self, size_t new_size,
    const void *default_value);
void vector_assign_impl(vector_t *self, size_t count, const void *value);

vector_t *vector_map_impl(const vector_t *self,
    map_function_t map_fn, size_t output_element_size, void *context);
vector_t *vector_filter_impl(const vector_t *self,
    predicate_function_t predicate, void *context);
void vector_reduce_impl(const vector_t *self,
    reduce_function_t reduce_fn, accumulator_t *acc, void *context);
void vector_for_each_impl(vector_t *self,
    for_each_function_t foreach_fn, void *context);
void vector_sort_impl(vector_t *self,
    compare_function_t compare_fn, void *context);
bool vector_any_impl(const vector_t *self,
    predicate_function_t predicate, void *context);
bool vector_all_impl(const vector_t *self,
    predicate_function_t predicate, void *context);
size_t vector_count_impl(const vector_t *self,
    predicate_function_t predicate, void *context);
void *vector_find_impl(const vector_t *self,
    predicate_function_t predicate, void *context);
vector_t *vector_reverse_impl(const vector_t *self);
vector_t *vector_slice_impl(const vector_t *self, size_t start, size_t end);

#endif /* !VECTOR_INTERNAL_H_ */
