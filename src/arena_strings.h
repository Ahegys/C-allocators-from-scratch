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
ArenaString8* arena_string8_capitalize(Arena* arena, const ArenaString8* str);
ArenaString8* arena_string8_to_upper_case(Arena* arena, const ArenaString8* str);
ArenaString8* arena_string8_to_lower_case(Arena* arena, const ArenaString8* str);
ArenaString8* arena_string8_trim(Arena* arena, const ArenaString8* str);
ArenaString8* arena_string8_trim_start(Arena* arena, const ArenaString8* str);
ArenaString8* arena_string8_trim_end(Arena* arena, const ArenaString8* str);
ArenaString8* arena_string8_replace(Arena* arena, const ArenaString8* str, const char* old_sub, const char* new_sub);
ArenaString8* arena_string8_replace_all(Arena* arena, const ArenaString8* str, const char* old_sub, const char* new_sub);
ArenaString8* arena_string8_slice(Arena* arena, const ArenaString8* str, int start, int end);
ArenaString8* arena_string8_substring(Arena* arena, const ArenaString8* str, size_t start, size_t end);
ArenaString8* arena_string8_substr(Arena* arena, const ArenaString8* str, size_t start, size_t length);
ArenaString8* arena_string8_pad_start(Arena* arena, const ArenaString8* str, size_t target_length, const char* pad_str);
ArenaString8* arena_string8_pad_end(Arena* arena, const ArenaString8* str, size_t target_length, const char* pad_str);
ArenaString8* arena_string8_repeat(Arena* arena, const ArenaString8* str, size_t count);
char arena_string8_char_at(const ArenaString8* str, size_t index);
char arena_string8_char_code_at(const ArenaString8* str, size_t index);
char arena_string8_at(const ArenaString8* str, int index);

// ArenaString16
ArenaString16* arena_string16_create(Arena* arena, const uint16_t* input);
ArenaString16* arena_string16_replace(Arena* arena, const ArenaString16* str, const uint16_t* old_sub, const uint16_t* new_sub);
ArenaString16* arena_string16_replace_all(Arena* arena, const ArenaString16* str, const uint16_t* old_sub, const uint16_t* new_sub);
ArenaString16* arena_string16_to_upper_case(Arena* arena, const ArenaString16* str);
ArenaString16* arena_string16_to_lower_case(Arena* arena, const ArenaString16* str);
ArenaString16* arena_string16_trim(Arena* arena, const ArenaString16* str);
ArenaString16* arena_string16_trim_start(Arena* arena, const ArenaString16* str);
ArenaString16* arena_string16_trim_end(Arena* arena, const ArenaString16* str);
ArenaString16* arena_string16_slice(Arena* arena, const ArenaString16* str, int start, int end);
ArenaString16* arena_string16_substring(Arena* arena, const ArenaString16* str, size_t start, size_t end);
ArenaString16* arena_string16_substr(Arena* arena, const ArenaString16* str, size_t start, size_t length);
ArenaString16* arena_string16_pad_start(Arena* arena, const ArenaString16* str, size_t target_length, const uint16_t* pad_str);
ArenaString16* arena_string16_pad_end(Arena* arena, const ArenaString16* str, size_t target_length, const uint16_t* pad_str);
ArenaString16* arena_string16_repeat(Arena* arena, const ArenaString16* str, size_t count);
uint16_t arena_string16_char_at(const ArenaString16* str, size_t index);
uint16_t arena_string16_char_code_at(const ArenaString16* str, size_t index);
uint16_t arena_string16_at(const ArenaString16* str, int index);

ArenaString32* arena_string32_create(Arena* arena, const uint32_t* input);
ArenaString32* arena_string32_replace(Arena* arena, const ArenaString32* str, const uint32_t* old_sub, const uint32_t* new_sub);
ArenaString32* arena_string32_replace_all(Arena* arena, const ArenaString32* str, const uint32_t* old_sub, const uint32_t* new_sub);
ArenaString32* arena_string32_to_upper_case(Arena* arena, const ArenaString32* str);
ArenaString32* arena_string32_to_lower_case(Arena* arena, const ArenaString32* str);
ArenaString32* arena_string32_trim(Arena* arena, const ArenaString32* str);
ArenaString32* arena_string32_trim_start(Arena* arena, const ArenaString32* str);
ArenaString32* arena_string32_trim_end(Arena* arena, const ArenaString32* str);
ArenaString32* arena_string32_slice(Arena* arena, const ArenaString32* str, int start, int end);
ArenaString32* arena_string32_substring(Arena* arena, const ArenaString32* str, size_t start, size_t end);
ArenaString32* arena_string32_substr(Arena* arena, const ArenaString32* str, size_t start, size_t length);
ArenaString32* arena_string32_pad_start(Arena* arena, const ArenaString32* str, size_t target_length, const uint32_t* pad_str);
ArenaString32* arena_string32_pad_end(Arena* arena, const ArenaString32* str, size_t target_length, const uint32_t* pad_str);
ArenaString32* arena_string32_repeat(Arena* arena, const ArenaString32* str, size_t count);
uint32_t arena_string32_char_at(const ArenaString32* str, size_t index);
uint32_t arena_string32_char_code_at(const ArenaString32* str, size_t index);
uint32_t arena_string32_at(const ArenaString32* str, int index);

ArenaString64* arena_string64_create(Arena* arena, const uint64_t* input);
ArenaString64* arena_string64_replace(Arena* arena, const ArenaString64* str, const uint64_t* old_sub, const uint64_t* new_sub);
ArenaString64* arena_string64_replace_all(Arena* arena, const ArenaString64* str, const uint64_t* old_sub, const uint64_t* new_sub);
ArenaString64* arena_string64_to_upper_case(Arena* arena, const ArenaString64* str);
ArenaString64* arena_string64_to_lower_case(Arena* arena, const ArenaString64* str);
ArenaString64* arena_string64_trim(Arena* arena, const ArenaString64* str);
ArenaString64* arena_string64_trim_start(Arena* arena, const ArenaString64* str);
ArenaString64* arena_string64_trim_end(Arena* arena, const ArenaString64* str);
ArenaString64* arena_string64_slice(Arena* arena, const ArenaString64* str, int start, int end);
ArenaString64* arena_string64_substring(Arena* arena, const ArenaString64* str, size_t start, size_t end);
ArenaString64* arena_string64_substr(Arena* arena, const ArenaString64* str, size_t start, size_t length);
ArenaString64* arena_string64_pad_start(Arena* arena, const ArenaString64* str, size_t target_length, const uint64_t* pad_str);
ArenaString64* arena_string64_pad_end(Arena* arena, const ArenaString64* str, size_t target_length, const uint64_t* pad_str);
ArenaString64* arena_string64_repeat(Arena* arena, const ArenaString64* str, size_t count);

uint64_t arena_string64_char_at(const ArenaString64* str, size_t index);
uint64_t arena_string64_char_code_at(const ArenaString64* str, size_t index);
uint64_t arena_string64_at(const ArenaString64* str, int index);

uint16_t* convert_to_uint16(Arena* arena, const char* str);
uint32_t* convert_to_uint32(Arena* arena, const char* str);
uint64_t* convert_to_uint64(Arena* arena, const char* str);

#endif // ARENA_STRINGS_H