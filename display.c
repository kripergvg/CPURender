#include "display.h"
#include <stdio.h>
#include "vector.h"
#include "triangle.h"

int window_width = 800;
int window_height = 600;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

float* z_buffer = NULL;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Couldn't intialize SDL");
		return false;
	}

	int displayIndex = 1;

	SDL_DisplayMode current;
	for (size_t i = 0; i < SDL_GetNumVideoDisplays(); i++)
	{
		SDL_GetCurrentDisplayMode(i, &current);
	}

	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(displayIndex, &display_mode);
	window_width = display_mode.w;
	window_height = display_mode.h;
	
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), SDL_WINDOWPOS_CENTERED_DISPLAY(displayIndex), window_width, window_height, SDL_WINDOW_BORDERLESS);
	if (!window) {
		fprintf(stderr, "Couldn't create window");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Couldn't create renderer");
		return false;
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void render_color_buffer(void) {
	SDL_UpdateTexture(
		color_buffer_texture,
		NULL,
		color_buffer,
		(int)window_width * sizeof(uint32_t)
	);

	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

void clear_color_buffer(uint32_t color) {
	for (size_t heightIndex = 0; heightIndex < window_height; heightIndex++)
	{
		for (size_t widthIndex = 0; widthIndex < window_width; widthIndex++)
		{
			int index = heightIndex * window_width + widthIndex;
			color_buffer[index] = color;
		}
	}
}

void clear_z_buffer(void) {
	for (size_t heightIndex = 0; heightIndex < window_height; heightIndex++)
	{
		for (size_t widthIndex = 0; widthIndex < window_width; widthIndex++)
		{
			int index = heightIndex * window_width + widthIndex;
			z_buffer[index] = 1;
		}
	}
}

void draw_grid(uint32_t color) {
	int gridSize = 10;
	for (size_t heightIndex = 0; heightIndex < window_height; heightIndex++)
	{
		for (size_t widthIndex = 0; widthIndex < window_width; widthIndex++)
		{
			if (heightIndex % gridSize == 0 || widthIndex % gridSize == 0) {
				int index = heightIndex * window_width + widthIndex;
				color_buffer[index] = color;
			}
		}
	}
}

void draw_rectangle(int xOffset, int yOffset, int width, int height, uint32_t color) {
	for (size_t heightIndex = yOffset; heightIndex < window_height && heightIndex - yOffset < height; heightIndex++)
	{
		for (size_t widthIndex = xOffset; widthIndex < window_width && widthIndex - xOffset < width; widthIndex++)
		{
			int index = heightIndex * window_width + widthIndex;
			vec2_int position = { widthIndex, heightIndex };
			draw_pixel(position, color);
		}
	}
}

void draw_pixel(vec2_int position, uint32_t color) {
	if (position.x >= 0 && position.x < window_width && position.y >= 0 && position.y < window_height) {
		color_buffer[position.y * window_width + position.x] = color;
	}
}

void draw_triangle(triangle_t triangle, uint32_t color) {
	draw_line(triangle.points[0].x, triangle.points[0].y, triangle.points[1].x, triangle.points[1].y, color);
	draw_line(triangle.points[1].x, triangle.points[1].y, triangle.points[2].x, triangle.points[2].y, color);
	draw_line(triangle.points[2].x, triangle.points[2].y, triangle.points[0].x, triangle.points[0].y, color);
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t color) {
	float dx = x2 - x1;
	float dy = y2 - y1;


	float biggerSideLength = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	float xStep = dx / biggerSideLength;
	float yStep = dy / biggerSideLength;
	int steps = biggerSideLength;

	float x = x1;
	float y = y1;

	for (size_t i = 0; i < steps; i++)
	{
		draw_rectangle(round(x), round(y), 1, 1, color);
		x += xStep;
		y += yStep;
	}
}

void destroy_wibndow(void) {
	free(color_buffer);
	free(z_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

