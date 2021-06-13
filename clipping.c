#include "clipping.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define PLATES_COUNT 6
plane_t frustrum_planes[PLATES_COUNT];

void init_frustum_planes(float horizontal_fov, float vertical_fov,  float z_near, float z_far) {
	float cos_vertical_half_fov = cos(vertical_fov / 2);
	float sin_vertical_half_fov = sin(vertical_fov / 2);

	float cos_horizontal_half_fov = cos(horizontal_fov / 2);
	float sin_horizontal_half_fov = sin(horizontal_fov / 2);

	vec3_t origin = { 0,0,0 };

	frustrum_planes[LEFT_FRUSTUM_PLANE].point = origin;
	frustrum_planes[LEFT_FRUSTUM_PLANE].normal = vec3_new(cos_horizontal_half_fov, 0, sin_horizontal_half_fov);

	frustrum_planes[RIGHT_FRUSTUM_PLANE].point = origin;
	frustrum_planes[RIGHT_FRUSTUM_PLANE].normal = vec3_new(-cos_horizontal_half_fov, 0, sin_horizontal_half_fov);

	frustrum_planes[TOP_FRUSTUM_PLANE].point = origin;
	frustrum_planes[TOP_FRUSTUM_PLANE].normal = vec3_new(0, -cos_vertical_half_fov, sin_vertical_half_fov);

	frustrum_planes[BOTTOM_FRUSTUM_PLANE].point = origin;
	frustrum_planes[BOTTOM_FRUSTUM_PLANE].normal = vec3_new(0, cos_vertical_half_fov, sin_vertical_half_fov);

	frustrum_planes[NEAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_near);
	frustrum_planes[NEAR_FRUSTUM_PLANE].normal = vec3_new(0, 0, 1);

	frustrum_planes[FAR_FRUSTUM_PLANE].point = vec3_new(0, 0, z_far);
	frustrum_planes[FAR_FRUSTUM_PLANE].normal = vec3_new(0, 0, -1);
}

polygon_t crete_polygon_from_triangle(vec3_t triangle[3], tex2_t texcoords[3]) {
	polygon_t result = {
		.vertices = {
			triangle[0],triangle[1],triangle[2]},
			.texcoords = {texcoords[0],texcoords[1],texcoords[2]},
			.vertices_count = 3
	};

	return result;
}

void clip_polygon_against_plane(polygon_t* polygon, int plane) {
	if (polygon->vertices_count == 0)
		return;

	vec3_t plane_point = frustrum_planes[plane].point;
	vec3_t plane_normal = frustrum_planes[plane].normal;

	vec3_t inside_vertices[MAX_VERTICES_IN_POLYGON];
	tex2_t inside_texcoords[MAX_VERTICES_IN_POLYGON];
	int inside_vertices_count = 0;

	int prev_vertex_index = polygon->vertices_count - 1;
	vec3_t* previous_vertex = &polygon->vertices[prev_vertex_index];
	float prev_dot = vec3_dot(plane_normal, vec3_sub(*previous_vertex, plane_point));
	tex2_t* prevoius_texcoord = &polygon->texcoords[prev_vertex_index];

	for (int vertex_index = 0; vertex_index < polygon->vertices_count; vertex_index++) {
		vec3_t* current_vertex = &polygon->vertices[vertex_index];
		tex2_t* current_texcoord = &polygon->texcoords[vertex_index];
		float current_dot = vec3_dot(plane_normal, vec3_sub(*current_vertex, plane_point));
		
		if (current_dot * prev_dot < 0) {
			float t = prev_dot / (prev_dot - current_dot);
			vec3_t intersection = vec3_add(*previous_vertex, vec3_mul(vec3_sub(*current_vertex, *previous_vertex), t));
			 
			inside_vertices[inside_vertices_count] = intersection;
			vec2_t pervoius_coords = vec2_from_tex2(*prevoius_texcoord);
			vec2_t current_coords = vec2_from_tex2(*current_texcoord);
			inside_texcoords[inside_vertices_count] = tex2_from_vec2(vec2_add(pervoius_coords, vec2_mul(vec2_sub(current_coords, pervoius_coords), t)));
			inside_vertices_count++;
		}

		if (current_dot > 0) {
			inside_vertices[inside_vertices_count] = *current_vertex;
			inside_texcoords[inside_vertices_count] = *current_texcoord;
			inside_vertices_count++;
		}

		previous_vertex = current_vertex;
		prevoius_texcoord = current_texcoord;
		prev_dot = current_dot;
	}

	for (size_t i = 0; i < inside_vertices_count; i++)
	{
		polygon->vertices[i] = inside_vertices[i];
		polygon->texcoords[i] = inside_texcoords[i];
	}

	polygon->vertices_count = inside_vertices_count;
}

void clip_polygon(polygon_t* p) {
	clip_polygon_against_plane(p, LEFT_FRUSTUM_PLANE);
	clip_polygon_against_plane(p, RIGHT_FRUSTUM_PLANE);
	clip_polygon_against_plane(p, TOP_FRUSTUM_PLANE);
	clip_polygon_against_plane(p, BOTTOM_FRUSTUM_PLANE);
	clip_polygon_against_plane(p, NEAR_FRUSTUM_PLANE);
	clip_polygon_against_plane(p, FAR_FRUSTUM_PLANE);
}

int polygon_to_triangles(polygon_t* polygon, triangle_f* triangles) {
	if (polygon->vertices_count == 0)
		return 0;

	for (int i = 0; i < polygon->vertices_count - 2; i++)
	{
		triangles[i].points[0] = polygon->vertices[0];
		triangles[i].texcoords[0] = polygon->texcoords[0];

		size_t index1 = i + 1;
		triangles[i].points[1] = polygon->vertices[index1]; 
		triangles[i].texcoords[1] = polygon->texcoords[index1];

		size_t index2 = i + 2;
		triangles[i].points[2] = polygon->vertices[index2];
		triangles[i].texcoords[2] = polygon->texcoords[index2];
	}

	return polygon->vertices_count - 2;
}