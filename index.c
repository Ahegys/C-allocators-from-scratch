#include "./src/arena_strings.h"  
#include "./src/memory.h"         
#include <stdio.h>                
#include <string.h>               
#include <stdint.h>

void test_assert_equal_str(const char* test_name, const char* expected, const char* actual) {
    if (strcmp(expected, actual) == 0) {
        printf("[PASS] %s: Expected '%s', got '%s'\n", test_name, expected, actual);
    } else {
        printf("[FAIL] %s: Expected '%s', got '%s'\n", test_name, expected, actual);
    }
}
void test_find_string8(Arena* arena) {
    ArenaString8* str = arena_string8_create(arena, "Hello, world!");
    printf("Enter Here\n");


    size_t index = arena_string8_find((const ArenaString8*)str, "world");

    if (index == 7) {
        printf("[PASS] arena_string8_find: Found 'world' at index %zu\n", index);
    } else {
        printf("[FAIL] arena_string8_find: Expected index 7, got %zu\n", index);
    }
}

void test_find_string16(Arena* arena) {
    uint16_t *input = convert_to_uint16(arena, "Hello, world!");
    ArenaString16* str = arena_string16_create(arena, input);

    uint16_t *sub = convert_to_uint16(arena, "world");
    size_t index = arena_string16_find(str, sub);

    if (index == 7) {
        printf("[PASS] arena_string16_find: Found 'world' at index %zu\n", index);
    } else {
        printf("[FAIL] arena_string16_find: Expected index 7, got %zu\n", index);
    }
}

void test_find_string32(Arena* arena) {
    ArenaString32* str = arena_string32_create(arena, convert_to_uint32(arena, "Hello World"));
    size_t index = arena_string32_find(str, convert_to_uint32(arena, "World"));

    if (index == 6) {
        printf("[PASS] arena_string32_find: Found 'World' at index %zu\n", index);
    } else {
        printf("[FAIL] arena_string32_find: Expected index 6, got %zu\n", index);
    }
}

void test_find_string64(Arena* arena) {
    ArenaString64* str = arena_string64_create(arena, convert_to_uint64(arena, "Testing 64"));
    size_t index = arena_string64_find(str, convert_to_uint64(arena, "64"));

    if (index == 8) {
        printf("[PASS] arena_string64_find: Found '64' at index %zu\n", index);
    } else {
        printf("[FAIL] arena_string64_find: Expected index 8, got %zu\n", index);
    }
}

void test_replace_string16(Arena* arena) {
    uint16_t *input = convert_to_uint16(arena, "Old Text");
    ArenaString16* original = arena_string16_create(arena, input);

    uint16_t *old_sub = convert_to_uint16(arena, "Old");
    uint16_t *new_sub = convert_to_uint16(arena, "New");

    ArenaString16* replaced = arena_string16_replace(arena, original, old_sub, new_sub);
    uint16_t *expected = convert_to_uint16(arena, "New Text");

    if (memcmp(replaced->data, expected, sizeof(&expected)) == 0) {
        printf("[PASS] arena_string16_replace: Replacement successful\n");
    } else {
        printf("[FAIL] arena_string16_replace: Replacement failed\n");
    }
}

void test_replace_string32(Arena* arena) {
    uint32_t *input = convert_to_uint32(arena, "ABCDEF");
    ArenaString32* original = arena_string32_create(arena, input);
    
    uint32_t *old_sub = convert_to_uint32(arena, "CD");
    uint32_t *new_sub = convert_to_uint32(arena, "XYZ");

    ArenaString32* replaced = arena_string32_replace(arena, original, old_sub, new_sub);
    uint32_t *expected = convert_to_uint32(arena, "ABXYZEF");

    if (memcmp(replaced->data, expected, sizeof(&expected)) == 0) {
        printf("[PASS] arena_string32_replace: Replacement successful\n");
    } else {
        printf("[FAIL] arena_string32_replace: Replacement failed\n");
    }
}

void test_replace_string64(Arena* arena) {
    uint64_t *input = convert_to_uint64(arena, "1234567");
    ArenaString64* original = arena_string64_create(arena, input);

    uint64_t *old_sub = convert_to_uint64(arena, "34");
    uint64_t *new_sub = convert_to_uint64(arena, "987");

    ArenaString64* replaced = arena_string64_replace(arena, original, old_sub, new_sub);
    uint64_t *expected = convert_to_uint64(arena, "12987567");

    if (memcmp(replaced->data, expected, sizeof(&expected)) == 0) {
        printf("[PASS] arena_string64_replace: Replacement successful\n");
    } else {
        printf("[FAIL] arena_string64_replace: Replacement failed\n");
    }
}


void run_arena_strings_tests() {
    Arena arena;

    INIT_ARENA(arena, 10); 
    
    printf("Running arena_strings tests...\n");
    
    test_find_string8(&arena);

    printf("Enter Here\n");
    test_find_string16(&arena);
    test_find_string32(&arena);
    test_find_string64(&arena);
    test_replace_string16(&arena);
    test_replace_string32(&arena);
    test_replace_string64(&arena);
    
    RESET_ARENA(&arena);
    printf("arena_strings tests completed.\n");
}

int main() {
    run_arena_strings_tests();
    return 0;
}
