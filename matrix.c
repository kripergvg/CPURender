#include "matrix.h"
#include <stdlib.h>
#include <math.h>

mat4_t mat4_identity(void) {
	mat4_t identity = {
		.m = {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,0},
			{0,0,0,1}
			}
	};

	return identity;
}


mat4_t mat4_scale(float x, float y, float z) {
	mat4_t scale = mat4_identity();
	scale.m[0][0] = x;
	scale.m[1][1] = y;
	scale.m[2][2] = z;

	return scale;
}


mat4_t mat4_look_at(vec3_t eye, vec3_t target, vec3_t up) {
	vec3_t z = vec3_sub(target, eye);
	vec3_normalize(&z);
	vec3_t x = vec3_cross(up, z);
	vec3_normalize(&x);
	vec3_t y = vec3_cross(z, x);

	mat4_t view_matrix = { {
		{x.x,x.y,x.z,-vec3_dot(x,eye)},
		{y.x,y.y,y.z,-vec3_dot(y,eye)},
		{z.x,z.y,z.z,-vec3_dot(z,eye)},
		{0,0,0,1}
	} };

	return view_matrix;
}

mat4_t mat4_translation(float x, float y, float z) {
	mat4_t translation = mat4_identity();
	translation.m[0][3] = x;
	translation.m[1][3] = y;
	translation.m[2][3] = z;
	return translation;
}

mat4_t mat4_z_rotation(float value) {
	mat4_t result = mat4_identity();
	result.m[0][0] = cos(value);
	result.m[1][0] = sin(value);

	result.m[0][1] = -sin(value);
	result.m[1][1] = cos(value);
	return result;
}

mat4_t mat4_y_rotation(float value) {
	mat4_t result = mat4_identity();
	result.m[0][0] = cos(value);
	result.m[2][0] = sin(value);

	result.m[0][2] = -sin(value);
	result.m[2][2] = cos(value);
	return result;
}

mat4_t mat4_x_rotation(float value) {
	mat4_t result = mat4_identity();
	result.m[2][2] = cos(value);
	result.m[1][2] = sin(value);

	result.m[2][1] = -sin(value);
	result.m[1][1] = cos(value);
	return result;
}

mat4_t mat4_rotation(float x, float y, float z) {
	mat4_t result =  mat4_mul(mat4_mul(mat4_x_rotation(x), mat4_y_rotation(y)), mat4_z_rotation(z));
	return result;
}

mat4_t mat4_perspective(float fov, float aspect, float znear, float zfar) {
	mat4_t m = { {{0}} };
	m.m[0][0] = aspect * (1 / tan(fov / 2));
	m.m[1][1] = 1 / tan(fov / 2);
	m.m[2][2] = zfar / (zfar - znear);
	m.m[2][3] = (-zfar * znear) / (zfar - znear);
	m.m[3][2] = 1.0;

	return m;
}

vec4_t mat4_mul_vec4(mat4_t mat, vec4_t vertex) {
	vec4_t result;
	result.x = mat.m[0][0] * vertex.x + mat.m[0][1] * vertex.y + mat.m[0][2] * vertex.z + mat.m[0][3] * vertex.w;
	result.y = mat.m[1][0] * vertex.x + mat.m[1][1] * vertex.y + mat.m[1][2] * vertex.z + mat.m[1][3] * vertex.w;
	result.z = mat.m[2][0] * vertex.x + mat.m[2][1] * vertex.y + mat.m[2][2] * vertex.z + mat.m[2][3] * vertex.w;
	result.w = mat.m[3][0] * vertex.x + mat.m[3][1] * vertex.y + mat.m[3][2] * vertex.z + mat.m[3][3] * vertex.w;

	return result;
}

vec4_t mat4_mul_vec4_project(mat4_t mat_proj, vec4_t v) {
	vec4_t result = mat4_mul_vec4(mat_proj, v);

	if (result.w != 0.0) {
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}

	return result;
}

mat4_t mat4_mul(mat4_t a, mat4_t b) {
	mat4_t result;
	for (size_t rowIndex = 0; rowIndex < 4; rowIndex++)
	{
		for (size_t columnIndex = 0; columnIndex < 4; columnIndex++)
		{
			result.m[rowIndex][columnIndex] = 0;
			for (size_t elementIndex = 0; elementIndex < 4; elementIndex++)
			{
				result.m[rowIndex][columnIndex] += a.m[rowIndex][elementIndex] * b.m[elementIndex][columnIndex];
			}
		}
	}

	return result;

}