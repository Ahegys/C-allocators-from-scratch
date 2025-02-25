#ifndef __TEST_ARENA_STRING_H__
#define __TEST_ARENA_STRING_H__
#include "arena_strings.h"
#include "screen_buffer.h"

void test_assert_equal_str(const char* test_name, const char* expected, const char* actual);
void test_find_string8(Arena* arena);
void test_find_string16(Arena* arena);
void test_find_string32(Arena* arena);
void test_find_string64(Arena* arena);
void test_replace_string16(Arena* arena);
void test_replace_string32(Arena* arena);
void test_replace_string64(Arena* arena);
void test_screen_buffer(Arena* arena);
void run_arena_strings_tests();
#endif