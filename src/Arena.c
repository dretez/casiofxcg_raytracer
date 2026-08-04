#include "Arena.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

Arena* Arena_create(size_t size) {
    Arena* arena = (Arena*)malloc(size);

    arena->size = size;
    arena->pos  = ARENA_BASE_POS;

    return arena;
}

void Arena_destroy(Arena* arena) {
    free(arena);
}

void* Arena_pushAligned(Arena* arena, size_t size, size_t align, bool nonzero) {
    size_t pos_aligned = ALIGN_UP_POW_2(arena->pos, align);
    size_t new_pos     = pos_aligned + size;

    if (new_pos > arena->size) return NULL;

    arena->pos = new_pos;

    u8* out = (u8*)arena + pos_aligned;

    if (!nonzero) memset(out, 0, size);

    return out;
}

void* Arena_push(Arena* arena, size_t size, bool nonzero) {
    return Arena_pushAligned(arena, size, ARENA_ALIGN, nonzero);
}

void Arena_pop(Arena* arena, size_t size) {
    size = min(size, arena->pos - ARENA_BASE_POS);
    arena->pos -= size;
}

void Arena_popTo(Arena* arena, size_t pos) {
    size_t size = pos < arena->pos ? arena->pos - pos : 0;
    Arena_pop(arena, size);
}

void Arena_clear(Arena* arena) {
    Arena_popTo(arena, ARENA_BASE_POS);
}
