#include "test_arena_string.h"
void test_assert_equal_str(const char* test_name, const char* expected, const char* actual) {
    if (strcmp(expected, actual) == 0) {
        printf("[PASS] %s: Expected '%s', got '%s'\n", test_name, expected, actual);
    } else {
        printf("[FAIL] %s: Expected '%s', got '%s'\n", test_name, expected, actual);
    }
}

void test_find_string8(Arena* arena) {
    ArenaString8* str = arena_string8_create(arena, "Hello, world!");
    size_t index = arena_string8_find((const ArenaString8*)str, "world");
    if (index == 7) {
        printf("[PASS] arena_string8_find: Found 'world' at index %zu\n", index);
    } else {
        printf("[FAIL] arena_string8_find: Expected index 7, got %zu\n", index);
    }
    ArenaString8 *data = arena_string8_capitalize(arena, str);
    printf("teste: [%s]\n", data->data);
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

void test_screen_buffer(Arena* arena) {
    printf("\nTestando o Screen Buffer com Janela...\n");

    setlocale(LC_ALL, "");

    wchar_t* screen_buffer = create_screen_buffer(arena);
    if (screen_buffer == NULL) {
        printf("Falha ao criar o screen buffer.\n");
        return;
    }

    write_to_screen_buffer(screen_buffer, 0, 0, L'H');
    write_to_screen_buffer(screen_buffer, 1, 0, L'e');
    write_to_screen_buffer(screen_buffer, 2, 0, L'l');
    write_to_screen_buffer(screen_buffer, 3, 0, L'l');
    write_to_screen_buffer(screen_buffer, 4, 0, L'o');
    write_to_screen_buffer(screen_buffer, 18, 4, L'ǁ');

    draw_window_border(screen_buffer);

    const wchar_t* content[3][3] = {
        {L"Nome", L"Idade", L"Cidade"},
        {L"João", L"25", L"São Paulo"},
        {L"João", L"25", L"São Paulo"}
    };
    draw_table_widget(screen_buffer, 2,1, 3, 3, content); 
    draw_table_widget(screen_buffer, 40,1, 3, 3, content);

    render_screen_buffer(screen_buffer);
}

void run_arena_strings_tests() {
    Arena arena;
    INIT_ARENA(arena, 100);

    printf("Running arena_strings tests...\n");
    test_find_string8(&arena);
    printf("Enter Here\n");
    test_find_string16(&arena);
    test_find_string32(&arena);
    test_find_string64(&arena);
    test_replace_string16(&arena);
    test_replace_string32(&arena);
    test_replace_string64(&arena);

    ArenaString8* str = arena_string8_create(&arena, "  Hello World  ");
    ArenaString8* upper = arena_string8_to_upper_case(&arena, str);
    printf("Upper: %s\n", upper->data);
    ArenaString8* trimmed = arena_string8_trim(&arena, str);
    printf("Trimmed: %s\n", trimmed->data);
    ArenaString8* padded = arena_string8_pad_start(&arena, str, 20, "123");
    printf("Padded: %s\n", padded->data);

    test_screen_buffer(&arena);

    RESET_ARENA(&arena);
    printf("arena_strings tests completed.\n");
}