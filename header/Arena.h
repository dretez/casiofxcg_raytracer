#ifndef INCLUDE_ARENA_H
#define INCLUDE_ARENA_H

#include <gint/defs/types.h>
#include <stddef.h>

#define KiB(n) ((u64)(n) << 10)

#define ARENA_BASE_POS (sizeof(Arena))
#define ARENA_ALIGN (sizeof(void*))

#define ALIGN_UP_POW_2(n, p) (((size_t)(n) + ((size_t)(p) - 1)) & (~((size_t)(p) - 1)))

typedef struct {
    size_t size;
    size_t pos;
} Arena;

Arena* Arena_create(size_t size);
void   Arena_destroy(Arena* arena);

void* Arena_pushAligned(Arena* arena, size_t size, size_t align, bool nonzero);
void* Arena_push(Arena* arena, size_t size, bool nonzero);
void  Arena_pop(Arena* arena, size_t size);
void  Arena_popTo(Arena* arena, size_t pos);
void  Arena_clear(Arena* arena);

#endif /* ifndef INCLUDE_ARENA_H */
