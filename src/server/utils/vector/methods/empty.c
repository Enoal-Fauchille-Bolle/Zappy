/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** empty
*/

#include "../vector_internal.h"
#include <assert.h>
#include <stdbool.h>

bool vector_empty_impl(const vector_t *self)
{
    assert(self);
    return self->size == 0;
}
