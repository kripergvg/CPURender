#pragma once
#include "vector.h"

typedef struct {
	float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);

mat4_t mat4_scale(float x, float y, float z);
mat4_t mat4_translation(float x, float y, float z);
mat4_t mat4_rotation(float x, float y, float z);
mat4_t mat4_perspective(float fov, float aspect, float znear, float zfar);
vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v);

mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up);

vec4_t mat4_mul_vec4(mat4_t mat, vec4_t vertex);

mat4_t mat4_mul(mat4_t a, mat4_t b);