#include "memory.h"
#include <stddef.h>

void arena_reset(Arena* arena) {
    arena->current = arena->start;
    arena->scopes_count = 0;
}
int arena_begin_scope(Arena* arena) {
    if (arena->scopes_count >= arena->scopes_size) {
        return -1;
    }
    arena->scopes[arena->scopes_count++] = arena->current;
    return 0;
}

int arena_end_scope(Arena* arena) {
    if (arena->scopes_count == 0) {
        return -1;
    }
    arena->current = arena->scopes[--arena->scopes_count];
    return 0;
}

void* arena_alloc(Arena* arena, UL size, UL alignment) {
    UL current = (UL)arena->current;
    UL aligned = (current + alignment - 1) & ~(alignment - 1);
    UL padding = aligned - current;

    if (arena->current + padding + size > arena->end) {
        return NULL;
    }

    arena->current += padding + size;
    return (void*)(arena->current - size);
}

Arena arena_init(UL scopes_size) {
    Arena arena;
    arena.start = memory_pool;
    arena.current = memory_pool;
    arena.end = memory_pool + MEMORY_POOL_SIZE;
    arena.scopes_count = 0;
    arena.scopes_size = scopes_size;

    arena.scopes = (char**)arena_alloc(&arena, scopes_size * sizeof(char*), sizeof(char*));
    if (arena.scopes == NULL) {
        Arena empty = {0};
        return empty;
    }
    return arena;
}