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
	vec4_t points[3];
	tex2_t texcoords[3];
	uint32_t color;
	float intensity;
	float avg_depth;
	int uniq;
} triangle_t;

typedef struct {
	vec2_int points[3];
} triangle_int;

typedef struct {
	int x;
	int y; 
	float z;
	float w;
} vertex_projected;

vec3_t get_normal(vec3_t triangle_vertices[3]);

void draw_filled_triangle(triangle_int triangle, uint32_t color);
void draw_flat_top(vec2_int min, vec2_int ver1, vec2_int ver2, uint32_t color);
void draw_flat_bottom(vec2_int max, vec2_int ver1, vec2_int ver2, uint32_t color);

void draw_textured_triangle(int x0, int y0, float z0, float w0, float u0, float v0,
	int x1, int y1, float z1, float w1, float u1, float v1,
	int x2, int y2, float z2, float w2, float u2, float v2,
	uint32_t* texture, float intensity);

void draw_texel(int x, int y, vec4_t a, vec4_t b, vec4_t c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv, uint32_t* texture, float intensity);
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);