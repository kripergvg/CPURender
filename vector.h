#pragma once

typedef struct {
	int x;
	int y;
} vec2_int;

typedef struct {
	float x;
	float y;
} vec2_t;

typedef struct {
	float x;
	float y;
	float z;
} vec3_t;

typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4_t;

typedef struct {
	int x;
	int y;
	float z;
	float w;
} vertex_projected;

vec3_t vec3_zero();

vec3_t vec3_rotate_x(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_z(vec3_t v, float angle);

vec3_t vec3_normalized(vec3_t a);
float vec3_dot(vec3_t a, vec3_t b);
vec3_t vec3_div(vec3_t a, float n);
vec3_t vec3_mul(vec3_t a, float n);
float vec3_magnitude(vec3_t a);
vec3_t vec3_add(vec3_t a, vec3_t b);
vec3_t vec3_sub(vec3_t a, vec3_t b);
vec3_t vec3_cross(vec3_t a, vec3_t b);

vec2_t vec2_normalized(vec2_t a, int* zero);
float vec2_dot(vec2_t a, vec2_t b);
vec2_t vec2_div(vec2_t a, float n);
vec2_t vec2_mul(vec2_t a, float n);
float vec2_magnitude(vec2_t a);
vec2_t vec2_sub(vec2_t a, vec2_t b);
vec2_t vec2_add(vec2_t a, vec2_t b);

vec3_t vec4_to_vec3(vec4_t v);
vec4_t vec3_to_vec4(vec3_t v);

vec2_t vec2_from_vec4(vec4_t v);

vec2_t vec2_from_projected(vertex_projected v);