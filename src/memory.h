#ifndef __MEMORY__
#define __MEMORY__

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

Arena arena_init(UL scopes_size);
void* arena_alloc(Arena* arena, UL size, UL alignment);
int arena_begin_scope(Arena* arena);
int arena_end_scope(Arena* arena);
void arena_reset(Arena* arena);

#define INIT_ARENA(arena, sc) {         \
    (arena) = arena_init(sc);             \
    if (arena.scopes == NULL) {             \
        fprintf(stderr, "Error to init arena\n"); \
        exit(1);                            \
    }                                         \
}

#define BEGIN_SCOPE(arena_ptr) {              \
    if (arena_begin_scope(arena_ptr) != 0) {    \
        fprintf(stderr, "Error to begin scope\n"); \
        exit(1);                              \
    }                                         \
}

#define ALLOC_ARENA(arena_ptr, count, type, var) {               \
    var = (type*)arena_alloc(arena_ptr, (count) * sizeof(type), sizeof(type)); \
    if (var == NULL) {                                           \
        fprintf(stderr, "Error to allocate memory\n");            \
        exit(1);                                                 \
    }                                                            \
}

#define END_SCOPE(arena_ptr) {                \
    if (arena_end_scope(arena_ptr) != 0) {      \
        fprintf(stderr, "Error to end scope\n"); \
        exit(1);                              \
    }                                         \
}

#define RESET_ARENA(arena_ptr) {              \
    arena_reset(arena_ptr);                   \
    printf("Arena reset\n"); \
}

#endif // __MEMORY__
