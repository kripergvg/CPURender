#pragma once

#include <SDL.h>
#include <stdbool.h>
#include "vector.h"
#include "triangle.h"

#define FPS 30
#define FRAME_TARGET_TIME (1000/FPS)

extern int window_width;
extern int window_height;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern float* z_buffer;

bool initialize_window(void);
void render_color_buffer(void);
void clear_color_buffer(uint32_t color);
void clear_z_buffer(void);

void draw_triangle(triangle_t triangle, uint32_t color);
void draw_grid(uint32_t color);
void draw_pixel(vec2_int position, uint32_t color);
void draw_rectangle(int xOffset, int yOffset, int width, int height, uint32_t color);
void draw_line(int x1, int y1, int x2, int y2, uint32_t color);
void destroy_wibndow(void);