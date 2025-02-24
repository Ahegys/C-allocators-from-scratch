#include "arena_strings.h"
#include <stdalign.h>
#include <string.h>

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

// Funções para criar strings
ArenaString8* arena_string8_create(Arena* arena, const char* input) {
    size_t len = string_length8(input);
    ArenaString8* str = (ArenaString8*) arena_alloc(arena, sizeof(ArenaString8), alignof(ArenaString8));
    str->length = len;
    str->capacity = len + 1;
    str->data = (char*) arena_alloc(arena, str->capacity, alignof(char));
    if (str->data) {
        memcpy(str->data, input, len);
        str->data[len] = '\0';
    }
    return str;
}

ArenaString16* arena_string16_create(Arena* arena, const uint16_t* input) {
    size_t len = string_length16(input);
    ArenaString16* str = (ArenaString16*) arena_alloc(arena, sizeof(ArenaString16), alignof(ArenaString16));
    str->length = len;
    str->capacity = len + 1;
    str->data = (uint16_t*) arena_alloc(arena, str->capacity * sizeof(uint16_t), alignof(uint16_t));
    if (str->data) {
        memcpy(str->data, input, len * sizeof(uint16_t));
        str->data[len] = 0;
    }
    return str;
}

ArenaString32* arena_string32_create(Arena* arena, const uint32_t* input) {
    size_t len = string_length32(input);
    ArenaString32* str = (ArenaString32*) arena_alloc(arena, sizeof(ArenaString32), alignof(ArenaString32));
    str->length = len;
    str->capacity = len + 1;
    str->data = (uint32_t*) arena_alloc(arena, str->capacity * sizeof(uint32_t), alignof(uint32_t));
    if (str->data) {
        memcpy(str->data, input, len * sizeof(uint32_t));
        str->data[len] = 0;
    }
    return str;
}

ArenaString64* arena_string64_create(Arena* arena, const uint64_t* input) {
    size_t len = string_length64(input);
    ArenaString64* str = (ArenaString64*) arena_alloc(arena, sizeof(ArenaString64), alignof(ArenaString64));
    str->length = len;
    str->capacity = len + 1;
    str->data = (uint64_t*) arena_alloc(arena, str->capacity * sizeof(uint64_t), alignof(uint64_t));
    if (str->data) {
        memcpy(str->data, input, len * sizeof(uint64_t));
        str->data[len] = 0;
    }
    return str;
}

ArenaString8* arena_string8_concat(Arena* arena, const ArenaString8* s1, const ArenaString8* s2) {
    size_t new_length = s1->length + s2->length;
    ArenaString8* new_str = (ArenaString8*) arena_alloc(arena, sizeof(ArenaString8), alignof(ArenaString8));
    new_str->length = new_length;
    new_str->capacity = new_length + 1;
    new_str->data = (char*) arena_alloc(arena, new_str->capacity, alignof(char));
    if (new_str->data) {
        memcpy(new_str->data, s1->data, s1->length);
        memcpy(new_str->data + s1->length, s2->data, s2->length);
        new_str->data[new_length] = '\0';
    }
    return new_str;
}

ArenaString16* arena_string16_concat(Arena* arena, const ArenaString16* s1, const ArenaString16* s2) {
    size_t new_length = s1->length + s2->length;
    ArenaString16* new_str = (ArenaString16*) arena_alloc(arena, sizeof(ArenaString16), alignof(ArenaString16));
    new_str->length = new_length;
    new_str->capacity = new_length + 1;
    new_str->data = (uint16_t*) arena_alloc(arena, new_str->capacity * sizeof(uint16_t), alignof(uint16_t));
    if (new_str->data) {
        memcpy(new_str->data, s1->data, s1->length * sizeof(uint16_t));
        memcpy(new_str->data + s1->length, s2->data, s2->length * sizeof(uint16_t));
        new_str->data[new_length] = 0;
    }
    return new_str;
}

ArenaString32* arena_string32_concat(Arena* arena, const ArenaString32* s1, const ArenaString32* s2) {
    size_t new_length = s1->length + s2->length;
    ArenaString32* new_str = (ArenaString32*) arena_alloc(arena, sizeof(ArenaString32), alignof(ArenaString32));
    new_str->length = new_length;
    new_str->capacity = new_length + 1;
    new_str->data = (uint32_t*) arena_alloc(arena, new_str->capacity * sizeof(uint32_t), alignof(uint32_t));
    if (new_str->data) {
        memcpy(new_str->data, s1->data, s1->length * sizeof(uint32_t));
        memcpy(new_str->data + s1->length, s2->data, s2->length * sizeof(uint32_t));
        new_str->data[new_length] = 0;
    }
    return new_str;
}

ArenaString64* arena_string64_concat(Arena* arena, const ArenaString64* s1, const ArenaString64* s2) {
    size_t new_length = s1->length + s2->length;
    ArenaString64* new_str = (ArenaString64*) arena_alloc(arena, sizeof(ArenaString64), alignof(ArenaString64));
    new_str->length = new_length;
    new_str->capacity = new_length + 1;
    new_str->data = (uint64_t*) arena_alloc(arena, new_str->capacity * sizeof(uint64_t), alignof(uint64_t));
    if (new_str->data) {
        memcpy(new_str->data, s1->data, s1->length * sizeof(uint64_t));
        memcpy(new_str->data + s1->length, s2->data, s2->length * sizeof(uint64_t));
        new_str->data[new_length] = 0;
    }
    return new_str;
}

#define DEFINE_CONVERT_FUNC(suffix, type)                                       \
type* convert_to_uint##suffix(Arena* arena, const char* str) {      \
    size_t len = strlen(str);         \
    type* result = (type*) arena_alloc(arena, (len + 1) * sizeof(type),          \
                                       alignof(type));                          \
    size_t i = 0, n = len;                                                     \
    for (; i + 3 < n; i += 4) {                                                \
        result[i]   = (type) str[i];                                           \
        result[i+1] = (type) str[i+1];                                         \
        result[i+2] = (type) str[i+2];                                         \
        result[i+3] = (type) str[i+3];                                         \
    }                                                                        \
    /* Processa o restante */                                                  \
    for (; i < n; i++) {                                                     \
        result[i] = (type) str[i];                                             \
    }                                                                        \
    result[len] = 0;               \
    return result;                                                           \
}

DEFINE_CONVERT_FUNC(16, uint16_t)
DEFINE_CONVERT_FUNC(32, uint32_t)
DEFINE_CONVERT_FUNC(64, uint64_t) 

#define DEFINE_FIND_FUNC(suffix, char_type, length_func) \
size_t arena_string##suffix##_find(const ArenaString##suffix* str, const char_type* sub) { \
    size_t sub_len = length_func(sub);  \
    if (sub_len == 0) return 0;           \
    if (sub_len > str->length) return SIZE_MAX;  \
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

// Funções replace corrigidas
ArenaString16* arena_string16_replace(Arena* arena, const ArenaString16* str, const uint16_t* old_sub, const uint16_t* new_sub) {
    size_t old_len = string_length16(old_sub);
    size_t new_len = string_length16(new_sub);
    if (old_len == 0) {
        return arena_string16_create(arena, str->data); 
    }

    size_t count = 0;
    const uint16_t* temp = str->data;
    size_t remaining = str->length;
    size_t pos;
    while ((pos = arena_string16_find(&(ArenaString16){(uint16_t*)temp, remaining, 0}, old_sub)) != SIZE_MAX) {
        count++;
        if (pos + old_len > remaining) break;
        temp += pos + old_len;
        remaining -= pos + old_len;
    }

    size_t new_length = str->length + count * (new_len - old_len);
    ArenaString16* new_str = (ArenaString16*) arena_alloc(arena, sizeof(ArenaString16), alignof(ArenaString16));
    new_str->length = new_length;
    new_str->capacity = new_length + 1;
    new_str->data = (uint16_t*) arena_alloc(arena, new_str->capacity * sizeof(uint16_t), alignof(uint16_t));
    if (new_str->data) {
        uint16_t* dest = new_str->data;
        const uint16_t* src = str->data;
        remaining = str->length;
        while ((pos = arena_string16_find(&(ArenaString16){(uint16_t*)src, remaining, 0}, old_sub)) != SIZE_MAX) {
            if (pos > remaining) break;
            size_t copy_len = pos;
            memcpy(dest, src, copy_len * sizeof(uint16_t));
            dest += copy_len;
            memcpy(dest, new_sub, new_len * sizeof(uint16_t));
            dest += new_len;
            src += pos + old_len;
            remaining -= pos + old_len;
        }
        memcpy(dest, src, remaining * sizeof(uint16_t));
        new_str->data[new_length] = 0;
    }
    return new_str;
}

ArenaString32* arena_string32_replace(Arena* arena, const ArenaString32* str, const uint32_t* old_sub, const uint32_t* new_sub) {
    size_t old_len = string_length32(old_sub);
    size_t new_len = string_length32(new_sub);
    if (old_len == 0) {
        return arena_string32_create(arena, str->data);
    }

    size_t count = 0;
    const uint32_t* temp = str->data;
    size_t remaining = str->length;
    size_t pos;
    while ((pos = arena_string32_find(&(ArenaString32){(uint32_t*)temp, remaining, 0}, old_sub)) != SIZE_MAX) {
        count++;
        if (pos + old_len > remaining) break;
        temp += pos + old_len;
        remaining -= pos + old_len;
    }

    size_t new_length = str->length + count * (new_len - old_len);
    ArenaString32* new_str = (ArenaString32*) arena_alloc(arena, sizeof(ArenaString32), alignof(ArenaString32));
    new_str->length = new_length;
    new_str->capacity = new_length + 1;
    new_str->data = (uint32_t*) arena_alloc(arena, new_str->capacity * sizeof(uint32_t), alignof(uint32_t));
    if (new_str->data) {
        uint32_t* dest = new_str->data;
        const uint32_t* src = str->data;
        remaining = str->length;
        while ((pos = arena_string32_find(&(ArenaString32){(uint32_t*)src, remaining, 0}, old_sub)) != SIZE_MAX) {
            if (pos > remaining) break;
            size_t copy_len = pos;
            memcpy(dest, src, copy_len * sizeof(uint32_t));
            dest += copy_len;
            memcpy(dest, new_sub, new_len * sizeof(uint32_t));
            dest += new_len;
            src += pos + old_len;
            remaining -= pos + old_len;
        }
        memcpy(dest, src, remaining * sizeof(uint32_t));
        new_str->data[new_length] = 0;
    }
    return new_str;
}

ArenaString64* arena_string64_replace(Arena* arena, const ArenaString64* str, const uint64_t* old_sub, const uint64_t* new_sub) {
    size_t old_len = string_length64(old_sub);
    size_t new_len = string_length64(new_sub);
    if (old_len == 0) {
        return arena_string64_create(arena, str->data);
    }

    size_t count = 0;
    const uint64_t* temp = str->data;
    size_t remaining = str->length;
    size_t pos;
    while ((pos = arena_string64_find(&(ArenaString64){(uint64_t*)temp, remaining, 0}, old_sub)) != SIZE_MAX) {
        count++;
        if (pos + old_len > remaining) break;
        temp += pos + old_len;
        remaining -= pos + old_len;
    }

    size_t new_length = str->length + count * (new_len - old_len);
    ArenaString64* new_str = (ArenaString64*) arena_alloc(arena, sizeof(ArenaString64), alignof(ArenaString64));
    new_str->length = new_length;
    new_str->capacity = new_length + 1;
    new_str->data = (uint64_t*) arena_alloc(arena, new_str->capacity * sizeof(uint64_t), alignof(uint64_t));
    if (new_str->data) {
        uint64_t* dest = new_str->data;
        const uint64_t* src = str->data;
        remaining = str->length;
        while ((pos = arena_string64_find(&(ArenaString64){(uint64_t*)src, remaining, 0}, old_sub)) != SIZE_MAX) {
            if (pos > remaining) break;
            size_t copy_len = pos;
            memcpy(dest, src, copy_len * sizeof(uint64_t));
            dest += copy_len;
            memcpy(dest, new_sub, new_len * sizeof(uint64_t));
            dest += new_len;
            src += pos + old_len;
            remaining -= pos + old_len;
        }
        memcpy(dest, src, remaining * sizeof(uint64_t));
        new_str->data[new_length] = 0;
    }
    return new_str;
}