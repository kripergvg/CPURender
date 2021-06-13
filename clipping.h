#pragma once
#include "vector.h"
#include "triangle.h"

enum {
	LEFT_FRUSTUM_PLANE,
	RIGHT_FRUSTUM_PLANE,
	TOP_FRUSTUM_PLANE,
	BOTTOM_FRUSTUM_PLANE,
	NEAR_FRUSTUM_PLANE,
	FAR_FRUSTUM_PLANE
};

typedef struct {
	vec3_t point;
	vec3_t normal;
} plane_t;

#define MAX_VERTICES_IN_POLYGON 10
#define MAX_TRIANGLES_IN_POLYGON 10

typedef struct {
	vec3_t vertices[MAX_VERTICES_IN_POLYGON];
	tex2_t texcoords[MAX_VERTICES_IN_POLYGON];
	int vertices_count;
} polygon_t;

void init_frustum_planes(float horizontal_fov, float vertical_fov, float z_near, float z_far);

polygon_t crete_polygon_from_triangle(vec3_t triangle[3], tex2_t texcoords[3]);

void clip_polygon(polygon_t* p);

int polygon_to_triangles(polygon_t* polygon, triangle_f* triangles);