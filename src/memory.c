#include "memory.h"
#include <stddef.h>

static ArenaResult make_success(void* data) {
    ArenaResult result;
    result.error = NULL;
    result.data = data;
    return result;
}

static ArenaResult make_error(int* error) {
    ArenaResult result;
    result.error = error;
    result.data = NULL;
    return result;
}

void arena_reset(Arena* arena) {
    if (arena == NULL) {
        fprintf(stderr, "arena_reset: NULL arena\n");
        return;
    }
    arena->current = arena->start;
    arena->scopes_count = 0;
}

ArenaResult arena_begin_scope(Arena* arena) {
    if (arena == NULL) {
        static int err = -2;
        return make_error(&err);
    }
    if (arena->scopes_count >= arena->scopes_size) {
        static int err = -1;
        return make_error(&err);
    }
    arena->scopes[arena->scopes_count++] = arena->current;
    return make_success(NULL);
}

ArenaResult arena_end_scope(Arena* arena) {
    if (arena == NULL) {
        static int err = -2;
        return make_error(&err);
    }
    if (arena->scopes_count == 0) {
        static int err = -1;
        return make_error(&err);
    }
    arena->current = arena->scopes[--arena->scopes_count];
    return make_success(NULL);
}

ArenaResult arena_alloc(Arena* arena, UL size, UL alignment) {
    if (arena == NULL) {
        static int err = -2;
        return make_error(&err);
    }
    UL current = (UL)arena->current;
    UL aligned = (current + alignment - 1) & ~(alignment - 1);
    UL padding = aligned - current;

    if (arena->current + padding + size > arena->end) {
        static int err = -1;
        return make_error(&err);
    }

    arena->current += padding + size;
    void* allocated = (void*)(arena->current - size);
    return make_success(allocated);
}

ArenaResult arena_init(Arena* arena, UL scopes_size) {
    if (arena == NULL) {
        static int err = -2;
        return make_error(&err);
    }
    arena->start = memory_pool;
    arena->current = memory_pool;
    arena->end = memory_pool + MEMORY_POOL_SIZE;
    arena->scopes_count = 0;
    arena->scopes_size = scopes_size;

    ArenaResult alloc_result = arena_alloc(arena, scopes_size * sizeof(char*), sizeof(char*));
    if (alloc_result.error) {
        return alloc_result;
    }
    arena->scopes = (char**)alloc_result.data;
    if (arena->scopes == NULL) {
        static int err = -3;
        return make_error(&err);
    }
    return make_success(arena);
}