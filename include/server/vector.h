/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** vector
*/

#ifndef VECTOR_H_
    #define VECTOR_H_

    #include <stddef.h>
    #include <stdbool.h>

// Define a custom vector data type
typedef void vector_item_t;

// Forward declaration - opaque types for encaplulation
typedef struct vector_s vector_t;
typedef struct vector_class_s vector_vtable_t;

// Function pointer types for callback functions
typedef void (*map_function_t)(const vector_item_t *input,
    vector_item_t *output, void *context);
typedef bool (*predicate_function_t)(const vector_item_t *element,
    void *context);
typedef void (*reduce_function_t)(void *accumulator,
    const vector_item_t *element, void *context);
typedef void (*for_each_function_t)(vector_item_t *element,
    size_t index, void *context);
typedef int (*compare_function_t)(const vector_item_t *a,
    const vector_item_t *b, void *context);

// Function pointer types to simulate vector method
typedef vector_item_t *(*vector_at_t)(const vector_t *self, size_t index);
typedef vector_item_t *(*vector_front_t)(const vector_t *self);
typedef vector_item_t *(*vector_back_t)(const vector_t *self);
typedef void *(*vector_data_t)(const vector_t *self);
typedef bool (*vector_empty_t)(const vector_t *self);
typedef size_t (*vector_size_t)(const vector_t *self);
typedef size_t (*vector_capacity_t)(const vector_t *self);
typedef void (*vector_reserve_t)(vector_t *self, size_t new_cap);
typedef void (*vector_shrink_to_fit_t)(vector_t *self);
typedef void (*vector_clear_t)(vector_t *self);
typedef void (*vector_push_back_t)(vector_t *self, const vector_item_t *item);
typedef void (*vector_pop_back_t)(vector_t *self);
typedef void (*vector_insert_t)(vector_t *self, size_t inder,
    const vector_item_t *item);
typedef void (*vector_erase_t)(vector_t *self, size_t index);
typedef void (*vector_resize_t)(vector_t *self, size_t new_size,
    const vector_item_t *default_value);
typedef void (*vector_assign_t)(vector_t *self, size_t count,
    const vector_item_t *value);

// Utility methods: functionnal paradigm
typedef struct {
    vector_item_t *initial_value;
    vector_item_t *result;
} accumulator_t;

typedef vector_t *(*vector_map_t)(const vector_t *self,
    map_function_t map_fn, size_t element_size, void *context);
typedef vector_t *(*vector_filter_t)(const vector_t *self,
    predicate_function_t predicate, void *context);
typedef void (*vector_reduce_t)(const vector_t *self,
    reduce_function_t reduce_fn, accumulator_t *acc, void *context);
typedef void (*vector_for_each_t)(vector_t *self,
    for_each_function_t foreach_fn, void *context);
typedef void (*vector_sort_t)(vector_t *self,
    compare_function_t compare_fn, void *context);
typedef bool (*vector_any_t)(const vector_t *self,
    predicate_function_t predicate, void *context);
typedef bool (*vector_all_t)(const vector_t *self,
    predicate_function_t predicate, void *context);
typedef size_t (*vector_count_t)(const vector_t *self,
    predicate_function_t predicate, void *context);
typedef vector_item_t *(*vector_find_t)(const vector_t *self,
    predicate_function_t predicate, void *context);
typedef vector_t *(*vector_reverse_t)(const vector_t *self);
typedef vector_t *(*vector_slice_t)(const vector_t *self,
    size_t start, size_t end);

struct vector_class_s {
    vector_at_t at;
    vector_front_t front;
    vector_back_t back;
    vector_data_t data;
    vector_empty_t empty;
    vector_size_t size;
    vector_capacity_t capacity;
    vector_reserve_t reserve;
    vector_shrink_to_fit_t shrink_to_fit;
    vector_clear_t clear;
    vector_push_back_t push_back;
    vector_pop_back_t pop_back;
    vector_insert_t insert;
    vector_erase_t erase;
    vector_resize_t resize;
    vector_assign_t assign;
    vector_map_t map;
    vector_filter_t filter;
    vector_reduce_t reduce;
    vector_for_each_t for_each;
    vector_sort_t sort;
    vector_any_t any;
    vector_all_t all;
    vector_count_t count;
    vector_find_t find;
    vector_reverse_t reverse;
    vector_slice_t slice;
};

extern const vector_vtable_t vector_class;

typedef struct {
    const vector_vtable_t *vtable;
} vector_public_t;

const vector_vtable_t *vector_get_vtable(vector_t *self);

vector_t *vector_new(size_t element_size);
void vector_destroy(vector_t *self);

#endif /* !VECTOR_H_ */
