#include "vector.h"
#include <math.h>

vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated = {
		v.x,
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)
	};

	return rotated;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated = {
		v.x* cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle)
	};

	return rotated;
}

vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated = {
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	};

	return rotated;
}

vec3_t vec3_zero() {
	vec3_t result = { 0,0,0 };
	return result;
}

vec3_t vec3_normalized(vec3_t a) {
	float length = vec3_magnitude(a);
	return vec3_div(a, length);
}

float vec3_dot(vec3_t a, vec3_t b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3_t vec3_div(vec3_t a, float n) {
	vec3_t result = { a.x / n,a.y / n, a.z / n };
	return result;
}

vec3_t vec3_mul(vec3_t a, float n) {
	vec3_t result = { a.x * n, a.y * n, a.z * n };
	return result;
}

float vec3_magnitude(vec3_t a) {
	return (float)sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) {
	vec3_t v = { a.x + b.x, a.y + b.y, a.z + b.z };
	return v;
}

vec3_t vec3_sub(vec3_t a, vec3_t b) {
	vec3_t v = { a.x - b.x, a.y - b.y, a.z - b.z };
	return v;
}

vec3_t vec3_cross(vec3_t a, vec3_t b) {
	vec3_t result = { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
	return result;
}



vec2_t vec2_normalized(vec2_t a) {
	float length = vec2_magnitude(a);
	return vec2_div(a, length);
}

float vec2_dot(vec2_t a, vec2_t b) {
	return a.x * b.x + a.y * b.y;
}

vec2_t vec2_div(vec2_t a, float n) {
	vec2_t result = { a.x / n,a.y / n };
	return result;
}

vec2_t vec2_mul(vec2_t a, float n) {
	vec2_t result = { a.x * n, a.y * n };
	return result;
}

float vec2_magnitude(vec2_t a) {
	return (float)sqrt(a.x * a.x + a.y * a.y);
}

vec2_t vec2_sub(vec2_t a, vec2_t b) {
	vec2_t v = { a.x - b.x, a.y - b.y };
	return v;
}

vec2_t vec2_add(vec2_t a, vec2_t b) {
	vec2_t v = { a.x + b.x, a.y + b.y };
	return v;
}



vec3_t vec4_to_vec3(vec4_t v) {
	vec3_t result = { v.x,v.y,v.z };
	return result;
}

vec4_t vec3_to_vec4(vec3_t v) {
	vec4_t result = { v.x,v.y,v.z,1 };
	return result;
}

vec2_t vec2_from_vec4(vec4_t v) {
	vec2_t result = { v.x,v.y };
	return result;
}