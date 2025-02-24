#ifndef ARENA_STRINGS_H
#define ARENA_STRINGS_H

#include <stddef.h>
#include <stdint.h>
#include "memory.h"

typedef struct {
    char* data;
    size_t length;
    size_t capacity;
} ArenaString8;
typedef struct {
    uint16_t* data;
    size_t length;
    size_t capacity;
} ArenaString16;

typedef struct {
    uint32_t* data;
    size_t length;
    size_t capacity;
} ArenaString32;

typedef struct {
    uint64_t* data;
    size_t length;
    size_t capacity;
} ArenaString64;


size_t arena_string8_find(const ArenaString8* str, const char* sub);
size_t arena_string16_find(const ArenaString16* str, const uint16_t* sub);
size_t arena_string32_find(const ArenaString32* str, const uint32_t* sub);
size_t arena_string64_find(const ArenaString64* str, const uint64_t* sub);


ArenaString8* arena_string8_create(Arena* arena, const char* input);
ArenaString16* arena_string16_create(Arena* arena, const uint16_t* input);
ArenaString32* arena_string32_create(Arena* arena, const uint32_t* input);
ArenaString64* arena_string64_create(Arena* arena, const uint64_t* input);
ArenaString16* arena_string16_replace(Arena* arena, const ArenaString16* str, const uint16_t* old_sub, const uint16_t* new_sub);
ArenaString32* arena_string32_replace(Arena* arena, const ArenaString32* str, const uint32_t* old_sub, const uint32_t* new_sub);
ArenaString64* arena_string64_replace(Arena* arena, const ArenaString64* str, const uint64_t* old_sub, const uint64_t* new_sub);


uint16_t *convert_to_uint16(Arena *arena, const char* str);
uint32_t *convert_to_uint32(Arena *arena, const char* str);
uint64_t *convert_to_uint64(Arena *arena, const char* str);
#endif // ARENA_STRINGS_H