#pragma once

#include "vector.h"
#include <stdint.h>
#include "texture.h"

typedef struct {
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	uint32_t color;
	int uniq;
} face_t;

typedef struct {
	vertex_projected points[3];
	tex2_t texcoords[3];
	uint32_t color;
	float intensity;
	int uniq;
} triangle_t;

typedef struct {
	vec2_int points[3];
} triangle_int;


vec3_t get_normal(vec3_t triangle_vertices[3]);

void draw_filled_triangle(vertex_projected vertex0, vertex_projected vertex1, vertex_projected vertex2,
	uint32_t color, float intensity);
void draw_flat_top(vec2_int min, vec2_int ver1, vec2_int ver2, uint32_t color);
void draw_flat_bottom(vec2_int max, vec2_int ver1, vec2_int ver2, uint32_t color);

void draw_pixel2(int x, int y, vertex_projected a, vertex_projected b, vertex_projected c, uint32_t color, float intensity);

void draw_textured_triangle(vertex_projected vertex0, float u0, float v0,
	vertex_projected vertex1, float u1, float v1,
	vertex_projected vertex2, float u2, float v2,
	uint32_t* texture, float intensity);

void draw_texel(int x, int y, vertex_projected a, vertex_projected b, vertex_projected c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv, uint32_t* texture, float intensity);
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

void vertex_swap(vertex_projected* a, vertex_projected* b);