#ifndef __MEMORY__
#define __MEMORY__

#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_POOL_SIZE (2 * 1024 * 1024)
typedef unsigned long UL;
static char memory_pool[MEMORY_POOL_SIZE];

typedef struct Arena {
    char* start;
    char* current;
    char* end;
    char** scopes;
    UL scopes_count;
    UL scopes_size;
} Arena;

ArenaResult arena_init(Arena* arena, UL scopes_size);
ArenaResult arena_alloc(Arena* arena, UL size, UL alignment);
ArenaResult arena_begin_scope(Arena* arena);
ArenaResult arena_end_scope(Arena* arena);
void arena_reset(Arena* arena);

#define INIT_ARENA(arena, sc) {                  \
    ArenaResult result = arena_init(&(arena), sc); \
    if (result.error) {                          \
        fprintf(stderr, "Error to init arena: %d\n", *result.error); \
        exit(1);                                 \
    }                                            \
    if (result.data != &(arena)) {               \
        fprintf(stderr, "Invalid arena pointer\n"); \
        exit(1);                                 \
    }                                            \
}

#define BEGIN_SCOPE(arena_ptr) {                 \
    ArenaResult result = arena_begin_scope(arena_ptr); \
    if (result.error) {                          \
        fprintf(stderr, "Error to begin scope: %d\n", *result.error); \
        exit(1);                                 \
    }                                            \
}

#define ALLOC_ARENA(arena_ptr, count, type, var) { \
    ArenaResult result = arena_alloc(arena_ptr, (count) * sizeof(type), sizeof(type)); \
    if (result.error) {                          \
        fprintf(stderr, "Error to allocate memory: %d\n", *result.error); \
        exit(1);                                 \
    }                                            \
    var = (type*)result.data;                    \
    if (var == NULL) {                           \
        fprintf(stderr, "Allocated pointer is NULL\n"); \
        exit(1);                                 \
    }                                            \
}

#define END_SCOPE(arena_ptr) {                   \
    ArenaResult result = arena_end_scope(arena_ptr); \
    if (result.error) {                          \
        fprintf(stderr, "Error to end scope: %d\n", *result.error); \
        exit(1);                                 \
    }                                            \
}

#define RESET_ARENA(arena_ptr) {                 \
    arena_reset(arena_ptr);                      \
    printf("Arena reset\n");                     \
}

#endif // __MEMORY__