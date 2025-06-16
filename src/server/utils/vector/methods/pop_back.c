/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** pop_back
*/

#include "../vector_internal.h"
#include <assert.h>

void vector_pop_back_impl(vector_t *self)
{
    assert(self && self->size > 0);
    self->size--;
    return;
}
