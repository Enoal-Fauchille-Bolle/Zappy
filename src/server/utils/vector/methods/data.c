/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** data
*/

#include "../vector_internal.h"
#include <assert.h>

void *vector_data_impl(const vector_t *self)
{
    assert(self);
    return self->data;
}
