#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <stdalign.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include "memory.h"
#include "screen_buffer.h"
#include <math.h>

static struct termios original_term;

// Configurações do mapa e tela
#define MAP_WIDTH 10
#define MAP_HEIGHT 10
#define SCREEN_WIDTH 200
#define SCREEN_HEIGHT 250
#define FOV 60.0 // Campo de visão em graus

// Mapa 2D (1 = parede, 0 = vazio)
static int map[MAP_HEIGHT][MAP_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,1,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

double posX = 2.0, posY = 2.0; 
double dirX = -1.0, dirY = 0.0;
double planeX = 0.0, planeY = 0.66;

void signal_handler(int sig) {
    printf("\033[?25h");
    restore_terminal();
    exit(0);
}

inline wchar_t* create_screen_buffer(Arena* arena) {
    ArenaResult buffer = arena_alloc(arena, SCREEN_BUFFER * sizeof(wchar_t), alignof(wchar_t));
    if (buffer.error) {
        perror("Error: Failed to allocate memory for screen buffer.\n");
        return NULL;
    }
    wmemset(buffer.data, L' ', SCREEN_BUFFER);
    return (wchar_t*)buffer.data;
}

static inline void write_to_buffer(wchar_t* buffer, int index, wchar_t c) {
    buffer[index] = c;
}

void configure_terminal() {
    struct termios newt;
    tcgetattr(STDIN_FILENO, &original_term);
    newt = original_term;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    printf("\033[?1003h");
    printf("\033[?25l");
    signal(SIGINT, signal_handler);
}

void restore_terminal() {
    printf("\033[?1003l");
    printf("\033[?25h");
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
    fcntl(STDIN_FILENO, F_SETFL, 0);
}

void process_input() {
    char buffer[3];
    int bytes_read;

    while ((bytes_read = read(STDIN_FILENO, buffer, 3)) > 0) {
        if (bytes_read == 1) {
            switch (buffer[0]) {
                case 'w':
                    if (map[(int)(posY + dirY * 0.1)][(int)posX] == 0) posY += dirY * 0.1;
                    if (map[(int)posY][(int)(posX + dirX * 0.1)] == 0) posX += dirX * 0.1;
                    break;
                case 's':
                    if (map[(int)(posY - dirY * 0.1)][(int)posX] == 0) posY -= dirY * 0.1;
                    if (map[(int)posY][(int)(posX - dirX * 0.1)] == 0) posX -= dirX * 0.1;
                    break;
                case 'a':
                    if (map[(int)(posY - planeY * 0.1)][(int)posX] == 0) posY -= planeY * 0.1;
                    if (map[(int)posY][(int)(posX - planeX * 0.1)] == 0) posX -= planeX * 0.1;
                    break;
                case 'd':
                    if (map[(int)(posY + planeY * 0.1)][(int)posX] == 0) posY += planeY * 0.1;
                    if (map[(int)posY][(int)(posX + planeX * 0.1)] == 0) posX += planeX * 0.1;
                    break;
            }
        } else if (bytes_read == 3 && buffer[0] == 27 && buffer[1] == '[') {
            switch (buffer[2]) {
                case 'D':
                    {
                        double oldDirX = dirX;
                        dirX = dirX * cos(-0.1) - dirY * sin(-0.1);
                        dirY = oldDirX * sin(-0.1) + dirY * cos(-0.1);
                        double oldPlaneX = planeX;
                        planeX = planeX * cos(-0.1) - planeY * sin(-0.1);
                        planeY = oldPlaneX * sin(-0.1) + planeY * cos(-0.1);
                    }
                    break;
                case 'C':
                    {
                        double oldDirX = dirX;
                        dirX = dirX * cos(0.1) - dirY * sin(0.1);
                        dirY = oldDirX * sin(0.1) + dirY * cos(0.1);
                        double oldPlaneX = planeX;
                        planeX = planeX * cos(0.1) - planeY * sin(0.1);
                        planeY = oldPlaneX * sin(0.1) + planeY * cos(0.1);
                    }
                    break;
            }
        }
    }
}
void draw_raycasting_widget(wchar_t* buffer, int start_x, int start_y, int width, int height) {
    const int stride = SCREEN_WIDTH;

    // Verifica limites
    if (start_x + width > SCREEN_WIDTH || start_y + height > SCREEN_HEIGHT) {
        perror("Error: Raycasting widget out of bounds.\n");
        return;
    }

    // Para cada coluna da tela
    for (int x = 0; x < width; x++) {
        double cameraX = 2 * x / (double)width - 1;
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        double sideDistX, sideDistY;
        int stepX, stepY;
        int hit = 0;
        int side;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }
        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        while (hit == 0) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT) break;
            if (map[mapY][mapX] > 0) hit = 1;
        }

        double perpWallDist;
        if (hit) {
            if (side == 0) perpWallDist = (mapX - posX + (1 - stepX) / 2.0) / rayDirX;
            else perpWallDist = (mapY - posY + (1 - stepY) / 2.0) / rayDirY;
        } else {
            perpWallDist = height;
        }

        int lineHeight = (int)(height / perpWallDist);
        int drawStart = -lineHeight / 2 + height / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + height / 2;
        if (drawEnd >= height) drawEnd = height - 1;

        for (int y = 0; y < height; y++) {
            int index = (start_y + y) * stride + (start_x + x);
            if (y < drawStart) {
                write_to_buffer(buffer, index, L' '); 
            } else if (y <= drawEnd) {
                write_to_buffer(buffer, index, L'█'); 
            } else {
                write_to_buffer(buffer, index, L' ');
            }
        }
    }
}

void draw_window_border(wchar_t* buffer) {
    const int w = WIDTH - 1;
    const int h = HEIGHT - 1;
    const int stride = WIDTH;

    write_to_buffer(buffer, 0, L'╔');
    write_to_buffer(buffer, w, L'╗');
    write_to_buffer(buffer, h * stride, L'╚');
    write_to_buffer(buffer, h * stride + w, L'╝');

    for (int x = 1; x < w; x++) {
        write_to_buffer(buffer, x, L'═');
        write_to_buffer(buffer, h * stride + x, L'═');
    }

    for (int y = 1; y < h; y++) {
        write_to_buffer(buffer, y * stride, L'║');
        write_to_buffer(buffer, y * stride + w, L'║');
    }
}

void render_screen_buffer(wchar_t* buffer) {
    static wchar_t output[SCREEN_BUFFER + HEIGHT + 1];
    int pos = 0;
    const int stride = WIDTH;

    for (int y = 0; y < HEIGHT; y++) {
        int offset = y * stride;
        for (int x = 0; x < WIDTH; x++) {
            output[pos++] = buffer[offset + x];
        }
        output[pos++] = L'\n';
    }
    output[pos] = L'\0';
    wprintf(L"%ls", output);
}

void render_screen_buffer_with_hover(wchar_t* buffer, int hover_x, int hover_y) {
    static wchar_t output[SCREEN_BUFFER + HEIGHT + 1024];
    int pos = 0;
    const int stride = WIDTH;
    const int col_width = 10;
    const int row_height = 2;

    wmemset(output, L' ', SCREEN_BUFFER + HEIGHT);
    for (int y = 0; y < HEIGHT; y++) {
        output[y * (stride + 1) + stride] = L'\n';
    }
    pos = swprintf(output, SCREEN_BUFFER + HEIGHT + 1024, L"\033[H");

    for (int y = 0; y < HEIGHT; y++) {
        int offset = y * stride;
        for (int x = 0; x < WIDTH; x++) {
            int table_start_x = 2;
            int table_start_y = 1;
            int col = (x - table_start_x) / (col_width + 1);
            int row = (y - table_start_y) / row_height;
            int cell_x_start = table_start_x + col * (col_width + 1);
            int cell_y_start = table_start_y + row * row_height;

            if (hover_x >= cell_x_start && hover_x < cell_x_start + col_width &&
                hover_y >= cell_y_start && hover_y < cell_y_start + row_height &&
                x >= cell_x_start && x < cell_x_start + col_width &&
                y >= cell_y_start && y < cell_y_start + row_height) {
                pos += swprintf(output + pos, SCREEN_BUFFER + HEIGHT + 1024 - pos, L"\033[93m%lc\033[0m", buffer[offset + x]);
            } else {
                output[pos++] = buffer[offset + x];
            }
        }
        output[pos++] = L'\n';
    }
    output[pos] = L'\0';
    wprintf(L"%ls", output);
    fflush(stdout);
}

static inline void write_string_to_buffer(wchar_t* buffer, int start_x, int y, const wchar_t* str, int col_width) {
    if (!str) return;
    int len = 0;
    while (str[len] && len < col_width) len++;
    int padding = (col_width - len) / 2;
    int base_index = y * WIDTH + start_x + padding;
    for (int i = 0; i < len && (start_x + padding + i < WIDTH); i++) {
        write_to_buffer(buffer, base_index + i, str[i]);
    }
}

void draw_table_widget(wchar_t* buffer, int start_x, int start_y, int rows, int cols, const wchar_t* content[rows][cols]) {
    const int col_width = 10;
    const int row_height = 1;
    const int col_step = col_width + 1;
    const int table_width = cols * col_step + 1;
    const int table_height = rows * (row_height + 1) + 1;
    const int stride = WIDTH;

    if (start_x + table_width > WIDTH || start_y + table_height > HEIGHT) {
        perror("Error: Table out of bounds.\n");
        return;
    }

    write_to_buffer(buffer, start_y * stride + start_x, L'┌');
    for (int c = 0; c < cols; c++) {
        int x_start = start_x + 1 + c * col_step;
        int top_offset = start_y * stride + x_start;
        for (int i = 0; i < col_width; i++) {
            write_to_buffer(buffer, top_offset + i, L'─');
        }
        if (c < cols - 1) {
            write_to_buffer(buffer, top_offset + col_width, L'┬');
        }
    }
    write_to_buffer(buffer, start_y * stride + start_x + table_width - 1, L'┐');

    for (int r = 0; r < rows; r++) {
        int row_y = start_y + r * (row_height + 1);
        int row_offset = row_y * stride;

        if (r > 0) {
            write_to_buffer(buffer, row_offset + start_x, L'├');
            for (int c = 0; c < cols; c++) {
                int x_start = start_x + 1 + c * col_step;
                for (int i = 0; i < col_width; i++) {
                    write_to_buffer(buffer, row_offset + x_start + i, L'─');
                }
                if (c < cols - 1) {
                    write_to_buffer(buffer, row_offset + x_start + col_width, L'┼');
                }
            }
            write_to_buffer(buffer, row_offset + start_x + table_width - 1, L'┤');
        }

        int content_y = row_y + 1;
        int content_offset = content_y * stride;
        for (int c = 0; c < cols; c++) {
            write_to_buffer(buffer, content_offset + start_x + c * col_step, L'│');
            if (content[r][c]) {
                write_string_to_buffer(buffer, start_x + c * col_step, content_y, content[r][c], col_width);
            }
            write_to_buffer(buffer, content_offset + start_x + table_width - 1, L'│');
        }
    }

    int bottom_y = start_y + table_height - 1;
    int bottom_offset = bottom_y * stride;
    write_to_buffer(buffer, bottom_offset + start_x, L'└');
    for (int c = 0; c < cols; c++) {
        int x_start = start_x + 1 + c * col_step;
        for (int i = 0; i < col_width; i++) {
            write_to_buffer(buffer, bottom_offset + x_start + i, L'─');
        }
        if (c < cols - 1) {
            write_to_buffer(buffer, bottom_offset + x_start + col_width, L'┴');
        }
    }
    write_to_buffer(buffer, bottom_offset + start_x + table_width - 1, L'┘');
}