#ifndef __SCREEN_BUFFER_H__
#define __SCREEN_BUFFER_H__
#include <stdio.h>                
#include <string.h>               
#include <stdint.h>
#include <wchar.h> 
#include <locale.h>
#include <stdalign.h>
#include "memory.h"
#define WIDTH 150
#define HEIGHT 15
#define SCREEN_BUFFER (WIDTH * HEIGHT)

wchar_t* create_screen_buffer(Arena* arena);
void write_to_screen_buffer(wchar_t* buffer, int x, int y, wchar_t c);
void draw_window_border(wchar_t* buffer);
void render_screen_buffer(wchar_t* buffer);
void write_string_to_screen_buffer(wchar_t* buffer, int x, int y, const wchar_t* str);
void draw_table_widget(wchar_t* buffer, int start_x, int start_y, int rows, int cols, const wchar_t* content[rows][cols]);
#endif