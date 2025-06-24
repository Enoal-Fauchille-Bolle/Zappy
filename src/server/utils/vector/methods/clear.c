/*
** EPITECH PROJECT, 2025
** Zappy_mirror
** File description:
** clear
*/

#include "../vector_internal.h"
#include <assert.h>

void vector_clear_impl(vector_t *self)
{
    assert(self);
    self->size = 0;
    return;
}
