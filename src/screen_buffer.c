#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stdalign.h>
#include "memory.h"
#include "screen_buffer.h"

wchar_t* create_screen_buffer(Arena* arena) {
    ArenaResult buffer = arena_alloc(arena, SCREEN_BUFFER * sizeof(wchar_t), alignof(wchar_t));
    if (buffer.error) {
        perror("Error: Failed to allocate memory for the screen buffer.\n");
        return NULL;
    }
    wmemset(buffer.data, L' ', SCREEN_BUFFER);
    return (wchar_t*)buffer.data;
}

void write_to_screen_buffer(wchar_t* buffer, int x, int y, wchar_t c) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        buffer[y * WIDTH + x] = c;
    } else {
        printf("Warning: Attempt to write out of bounds at (%d, %d)\n", x, y);
    }
}

void draw_window_border(wchar_t* buffer) {
    write_to_screen_buffer(buffer, 0, 0, L'╔');
    write_to_screen_buffer(buffer, WIDTH - 1, 0, L'╗');
    write_to_screen_buffer(buffer, 0, HEIGHT - 1, L'╚');
    write_to_screen_buffer(buffer, WIDTH - 1, HEIGHT - 1, L'╝');
    for (int x = 1; x < WIDTH - 1; x++) {
        write_to_screen_buffer(buffer, x, 0, L'═');
        write_to_screen_buffer(buffer, x, HEIGHT - 1, L'═');
    }
    for (int y = 1; y < HEIGHT - 1; y++) {
        write_to_screen_buffer(buffer, 0, y, L'║');
        write_to_screen_buffer(buffer, WIDTH - 1, y, L'║');
    }
}

void render_screen_buffer(wchar_t* buffer) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            putwchar(buffer[y * WIDTH + x]);
        }
        putwchar(L'\n');
    }
}

void write_string_to_screen_buffer(wchar_t* buffer, int x, int y, const wchar_t* str) {
    if (!str) {
        printf("Error: Null string at (%d, %d)\n", x, y);
        return;
    }
    int len = wcslen(str);
    for (int i = 0; i < len && x + i < WIDTH; i++) {
        write_to_screen_buffer(buffer, x + i, y, str[i]);
    }
}

void draw_table_widget(wchar_t* buffer, int start_x, int start_y, int rows, int cols, const wchar_t* content[rows][cols]) {
    int col_width = 10;
    int row_height = 1;
    int table_width = cols * col_width + cols + 1;
    int table_height = rows * row_height + rows + 1;


    if (start_x + table_width > WIDTH || start_y + table_height > HEIGHT) {
        perror("Error: Table out of bounds of the buffer.\n");
        return;
    }

    write_to_screen_buffer(buffer, start_x, start_y, L'┌');
    for (int c = 0; c < cols; c++) {
        for (int i = 0; i < col_width; i++) {
            write_to_screen_buffer(buffer, start_x + 1 + c * (col_width + 1) + i, start_y, L'─');
        }
        if (c < cols - 1) {
            write_to_screen_buffer(buffer, start_x + 1 + c * (col_width + 1) + col_width, start_y, L'┬');
        }
    }
    write_to_screen_buffer(buffer, start_x + table_width - 1, start_y, L'┐');

    for (int r = 0; r < rows; r++) {
        if (r > 0) {
            write_to_screen_buffer(buffer, start_x, start_y + r * (row_height + 1), L'├');
            for (int c = 0; c < cols; c++) {
                for (int i = 0; i < col_width; i++) {
                    write_to_screen_buffer(buffer, start_x + 1 + c * (col_width + 1) + i, start_y + r * (row_height + 1), L'─');
                }
                if (c < cols - 1) {
                    write_to_screen_buffer(buffer, start_x + 1 + c * (col_width + 1) + col_width, start_y + r * (row_height + 1), L'┼');
                }
            }
            write_to_screen_buffer(buffer, start_x + table_width - 1, start_y + r * (row_height + 1), L'┤');
        }
        for (int c = 0; c < cols; c++) {
            write_to_screen_buffer(buffer, start_x + c * (col_width + 1), start_y + r * (row_height + 1) + 1, L'│');
            if (content[r][c]) {
                int len = wcslen(content[r][c]);
                int padding = (col_width - len) / 2;
                int text_x = start_x + 1 + c * (col_width + 1) + padding;
                write_string_to_screen_buffer(buffer, text_x, start_y + r * (row_height + 1) + 1, content[r][c]);
            } else {
                printf("Warning: Null content at (%d, %d)\n", r, c);
            }
        }
        write_to_screen_buffer(buffer, start_x + table_width - 1, start_y + r * (row_height + 1) + 1, L'│');
    }

    write_to_screen_buffer(buffer, start_x, start_y + table_height - 1, L'└');
    for (int c = 0; c < cols; c++) {
        for (int i = 0; i < col_width; i++) {
            write_to_screen_buffer(buffer, start_x + 1 + c * (col_width + 1) + i, start_y + table_height - 1, L'─');
        }
        if (c < cols - 1) {
            write_to_screen_buffer(buffer, start_x + 1 + c * (col_width + 1) + col_width, start_y + table_height - 1, L'┴');
        }
    }
    write_to_screen_buffer(buffer, start_x + table_width - 1, start_y + table_height - 1, L'┘');
}