#include "arena_strings.h"
#include "memory.h"
#include "util.h"
#include <stdalign.h>
#include <string.h>
#include <stdint.h>

static size_t string_length8(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') len++;
    return len;
}

static size_t string_length16(const uint16_t* str) {
    size_t len = 0;
    while (str[len] != 0) len++;
    return len;
}

static size_t string_length32(const uint32_t* str) {
    size_t len = 0;
    while (str[len] != 0) len++;
    return len;
}

static size_t string_length64(const uint64_t* str) {
    size_t len = 0;
    while (str[len] != 0) len++;
    return len;
}

static char to_upper8(char c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

static char to_lower8(char c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

static uint16_t to_upper16(uint16_t c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

static uint16_t to_lower16(uint16_t c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

static uint32_t to_upper32(uint32_t c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

static uint32_t to_lower32(uint32_t c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

static uint64_t to_upper64(uint64_t c) {
    return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
}

static uint64_t to_lower64(uint64_t c) {
    return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
}

#define DEFINE_STRING_CREATE(suffix, type, length_func) \
ArenaString##suffix* arena_string##suffix##_create(Arena* arena, const type* input) { \
    size_t len = length_func(input); \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* str = (ArenaString##suffix*)alloc_result.data; \
    str->length = len; \
    str->capacity = len + 1; \
    ArenaResult data_result = arena_alloc(arena, str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    str->data = (type*)data_result.data; \
    memcpy(str->data, input, len * sizeof(type)); \
    str->data[len] = 0; \
    return str; \
}

DEFINE_STRING_CREATE(8, char, string_length8)
DEFINE_STRING_CREATE(16, uint16_t, string_length16)
DEFINE_STRING_CREATE(32, uint32_t, string_length32)
DEFINE_STRING_CREATE(64, uint64_t, string_length64)

#define DEFINE_FIND_FUNC(suffix, char_type, length_func) \
size_t arena_string##suffix##_find(const ArenaString##suffix* str, const char_type* sub) { \
    size_t sub_len = length_func(sub); \
    if (sub_len == 0) return 0; \
    if (sub_len > str->length) return SIZE_MAX; \
    for (size_t i = 0; i <= str->length - sub_len; i++) { \
        if (memcmp(str->data + i, sub, sub_len * sizeof(char_type)) == 0) { \
            return i; \
        } \
    } \
    return SIZE_MAX; \
}

DEFINE_FIND_FUNC(8, char, string_length8)
DEFINE_FIND_FUNC(16, uint16_t, string_length16)
DEFINE_FIND_FUNC(32, uint32_t, string_length32)
DEFINE_FIND_FUNC(64, uint64_t, string_length64)

#define DEFINE_CONVERT_FUNC(suffix, type) \
type* convert_to_uint##suffix(Arena* arena, const char* str) { \
    size_t len = strlen(str); \
    ArenaResult alloc_result = arena_alloc(arena, (len + 1) * sizeof(type), alignof(type)); \
    if (alloc_result.error) return NULL; \
    type* result = (type*)alloc_result.data; \
    for (size_t i = 0; i < len; i++) { \
        result[i] = (type)str[i]; \
    } \
    result[len] = 0; \
    return result; \
}

DEFINE_CONVERT_FUNC(16, uint16_t)
DEFINE_CONVERT_FUNC(32, uint32_t)
DEFINE_CONVERT_FUNC(64, uint64_t)

#define DEFINE_STRING_METHOD(suffix, type, to_upper_func, to_lower_func) \
type arena_string##suffix##_char_at(const ArenaString##suffix* str, size_t index) { \
    if (index >= str->length) return 0; \
    return str->data[index]; \
} \
type arena_string##suffix##_char_code_at(const ArenaString##suffix* str, size_t index) { \
    return arena_string##suffix##_char_at(str, index); \
} \
type arena_string##suffix##_at(const ArenaString##suffix* str, int index) { \
    if (index < 0) index = str->length + index; \
    if (index < 0 || (size_t)index >= str->length) return 0; \
    return str->data[index]; \
} \
ArenaString##suffix* arena_string##suffix##_slice(Arena* arena, const ArenaString##suffix* str, int start, int end) { \
    if (start < 0) start = str->length + start; \
    if (end < 0) end = str->length + end; \
    if (start < 0) start = 0; \
    if (end < 0 || end > (int)str->length) end = str->length; \
    if (start >= end) return arena_string##suffix##_create(arena, (type[]){0}); \
    size_t len = end - start; \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = len; \
    new_str->capacity = len + 1; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    memcpy(new_str->data, str->data + start, len * sizeof(type)); \
    new_str->data[len] = 0; \
    return new_str; \
} \
ArenaString##suffix* arena_string##suffix##_substring(Arena* arena, const ArenaString##suffix* str, size_t start, size_t end) { \
    if (start > str->length) start = str->length; \
    if (end > str->length) end = str->length; \
    if (start > end) { size_t tmp = start; start = end; end = tmp; } \
    return arena_string##suffix##_slice(arena, str, start, end); \
} \
ArenaString##suffix* arena_string##suffix##_substr(Arena* arena, const ArenaString##suffix* str, size_t start, size_t length) { \
    if (start >= str->length) return arena_string##suffix##_create(arena, (type[]){0}); \
    if (start + length > str->length) length = str->length - start; \
    return arena_string##suffix##_slice(arena, str, start, start + length); \
} \
ArenaString##suffix* arena_string##suffix##_to_upper_case(Arena* arena, const ArenaString##suffix* str) { \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = str->length; \
    new_str->capacity = str->capacity; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    for (size_t i = 0; i < str->length; i++) { \
        new_str->data[i] = to_upper_func(str->data[i]); \
    } \
    new_str->data[str->length] = 0; \
    return new_str; \
} \
ArenaString##suffix* arena_string##suffix##_to_lower_case(Arena* arena, const ArenaString##suffix* str) { \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = str->length; \
    new_str->capacity = str->capacity; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    for (size_t i = 0; i < str->length; i++) { \
        new_str->data[i] = to_lower_func(str->data[i]); \
    } \
    new_str->data[str->length] = 0; \
    return new_str; \
} \
ArenaString##suffix* arena_string##suffix##_trim(Arena* arena, const ArenaString##suffix* str) { \
    size_t start = 0, end = str->length; \
    while (start < str->length && str->data[start] == ' ') start++; \
    while (end > start && str->data[end - 1] == ' ') end--; \
    return arena_string##suffix##_slice(arena, str, start, end); \
} \
ArenaString##suffix* arena_string##suffix##_trim_start(Arena* arena, const ArenaString##suffix* str) { \
    size_t start = 0; \
    while (start < str->length && str->data[start] == ' ') start++; \
    return arena_string##suffix##_slice(arena, str, start, str->length); \
} \
ArenaString##suffix* arena_string##suffix##_trim_end(Arena* arena, const ArenaString##suffix* str) { \
    size_t end = str->length; \
    while (end > 0 && str->data[end - 1] == ' ') end--; \
    return arena_string##suffix##_slice(arena, str, 0, end); \
} \
ArenaString##suffix* arena_string##suffix##_pad_start(Arena* arena, const ArenaString##suffix* str, size_t target_length, const type* pad_str) { \
    size_t pad_len = string_length##suffix(pad_str); \
    if (target_length <= str->length || pad_len == 0) return arena_string##suffix##_create(arena, str->data); \
    size_t pad_count = (target_length - str->length + pad_len - 1) / pad_len; \
    size_t new_length = str->length + pad_count * pad_len; \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = new_length; \
    new_str->capacity = new_length + 1; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    size_t offset = 0; \
    for (size_t i = 0; i < pad_count; i++) { \
        memcpy(new_str->data + offset, pad_str, pad_len * sizeof(type)); \
        offset += pad_len; \
    } \
    memcpy(new_str->data + (new_length - str->length), str->data, str->length * sizeof(type)); \
    new_str->data[new_length] = 0; \
    return arena_string##suffix##_slice(arena, new_str, 0, target_length); \
} \
ArenaString##suffix* arena_string##suffix##_pad_end(Arena* arena, const ArenaString##suffix* str, size_t target_length, const type* pad_str) { \
    size_t pad_len = string_length##suffix(pad_str); \
    if (target_length <= str->length || pad_len == 0) return arena_string##suffix##_create(arena, str->data); \
    size_t pad_count = (target_length - str->length + pad_len - 1) / pad_len; \
    size_t new_length = str->length + pad_count * pad_len; \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = new_length; \
    new_str->capacity = new_length + 1; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    memcpy(new_str->data, str->data, str->length * sizeof(type)); \
    size_t offset = str->length; \
    for (size_t i = 0; i < pad_count; i++) { \
        memcpy(new_str->data + offset, pad_str, pad_len * sizeof(type)); \
        offset += pad_len; \
    } \
    new_str->data[new_length] = 0; \
    return arena_string##suffix##_slice(arena, new_str, 0, target_length); \
} \
ArenaString##suffix* arena_string##suffix##_repeat(Arena* arena, const ArenaString##suffix* str, size_t count) { \
    if (count == 0) return arena_string##suffix##_create(arena, (type[]){0}); \
    size_t new_length = str->length * count; \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = new_length; \
    new_str->capacity = new_length + 1; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    for (size_t i = 0; i < count; i++) { \
        memcpy(new_str->data + i * str->length, str->data, str->length * sizeof(type)); \
    } \
    new_str->data[new_length] = 0; \
    return new_str; \
} \
ArenaString##suffix* arena_string##suffix##_replace(Arena* arena, const ArenaString##suffix* str, const type* old_sub, const type* new_sub) { \
    size_t old_len = string_length##suffix(old_sub); \
    size_t new_len = string_length##suffix(new_sub); \
    if (old_len == 0) return arena_string##suffix##_create(arena, str->data); \
    size_t pos = arena_string##suffix##_find(str, old_sub); \
    if (pos == SIZE_MAX) return arena_string##suffix##_create(arena, str->data); \
    size_t new_length = str->length - old_len + new_len; \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = new_length; \
    new_str->capacity = new_length + 1; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    memcpy(new_str->data, str->data, pos * sizeof(type)); \
    memcpy(new_str->data + pos, new_sub, new_len * sizeof(type)); \
    memcpy(new_str->data + pos + new_len, str->data + pos + old_len, (str->length - pos - old_len) * sizeof(type)); \
    new_str->data[new_length] = 0; \
    return new_str; \
} \
ArenaString##suffix* arena_string##suffix##_replace_all(Arena* arena, const ArenaString##suffix* str, const type* old_sub, const type* new_sub) { \
    size_t old_len = string_length##suffix(old_sub); \
    size_t new_len = string_length##suffix(new_sub); \
    if (old_len == 0) return arena_string##suffix##_create(arena, str->data); \
    size_t count = 0; \
    const type* temp = str->data; \
    size_t remaining = str->length; \
    size_t pos; \
    while ((pos = arena_string##suffix##_find(&(ArenaString##suffix){(type*)temp, remaining, 0}, old_sub)) != SIZE_MAX) { \
        count++; \
        if (pos + old_len > remaining) break; \
        temp += pos + old_len; \
        remaining -= pos + old_len; \
    } \
    size_t new_length = str->length + count * (new_len - old_len); \
    ArenaResult alloc_result = arena_alloc(arena, sizeof(ArenaString##suffix), alignof(ArenaString##suffix)); \
    if (alloc_result.error) return NULL; \
    ArenaString##suffix* new_str = (ArenaString##suffix*)alloc_result.data; \
    new_str->length = new_length; \
    new_str->capacity = new_length + 1; \
    ArenaResult data_result = arena_alloc(arena, new_str->capacity * sizeof(type), alignof(type)); \
    if (data_result.error) return NULL; \
    new_str->data = (type*)data_result.data; \
    type* dest = new_str->data; \
    const type* src = str->data; \
    remaining = str->length; \
    while ((pos = arena_string##suffix##_find(&(ArenaString##suffix){(type*)src, remaining, 0}, old_sub)) != SIZE_MAX) { \
        if (pos > remaining) break; \
        size_t copy_len = pos; \
        memcpy(dest, src, copy_len * sizeof(type)); \
        dest += copy_len; \
        memcpy(dest, new_sub, new_len * sizeof(type)); \
        dest += new_len; \
        src += pos + old_len; \
        remaining -= pos + old_len; \
    } \
    memcpy(dest, src, remaining * sizeof(type)); \
    new_str->data[new_length] = 0; \
    return new_str; \
}

DEFINE_STRING_METHOD(8, char, to_upper8, to_lower8)
DEFINE_STRING_METHOD(16, uint16_t, to_upper16, to_lower16)
DEFINE_STRING_METHOD(32, uint32_t, to_upper32, to_lower32)
DEFINE_STRING_METHOD(64, uint64_t, to_upper64, to_lower64)

// Função capitalize (adicionada explicitamente)
ArenaString8* arena_string8_capitalize(Arena* arena, const ArenaString8* str) {
    ArenaString8* new_str = arena_string8_create(arena, str->data);
    if (new_str && new_str->length > 0) {
        new_str->data[0] = to_upper8(new_str->data[0]);
    }
    return new_str;
}

typedef struct {
    ArenaString8** strings;
    size_t count;
} ArenaString8Array;

ArenaString8Array arena_string8_split(Arena* arena, const ArenaString8* str, const char* separator) {
    size_t sep_len = string_length8(separator);
    if (sep_len == 0) {
        ArenaString8Array result = {NULL, 0};
        return result;
    }
    size_t count = 0;
    const char* temp = str->data;
    size_t remaining = str->length;
    size_t pos;
    while ((pos = arena_string8_find(&(ArenaString8){(char*)temp, remaining, 0}, separator)) != SIZE_MAX) {
        count++;
        if (pos + sep_len > remaining) break;
        temp += pos + sep_len;
        remaining -= pos + sep_len;
    }
    count++; 
    ArenaResult alloc_result = arena_alloc(arena, count * sizeof(ArenaString8*), alignof(ArenaString8*));
    if (alloc_result.error) {
        ArenaString8Array result = {NULL, 0};
        return result;
    }
    ArenaString8** strings = (ArenaString8**)alloc_result.data;
    temp = str->data;
    remaining = str->length;
    size_t i = 0;
    size_t start = 0;
    while ((pos = arena_string8_find(&(ArenaString8){(char*)temp, remaining, 0}, separator)) != SIZE_MAX) {
        strings[i] = arena_string8_slice(arena, str, start, start + pos);
        start += pos + sep_len;
        temp += pos + sep_len;
        remaining -= pos + sep_len;
        i++;
    }
    strings[i] = arena_string8_slice(arena, str, start, str->length);
    ArenaString8Array result = {strings, count};
    return result;
}