#include "triangle.h"
#include "vector.h"
#include "display.h"
#include "light.h"

vec3_t get_normal(vec3_t triangle_vertices[3]) {
	vec3_t origin = triangle_vertices[0];
	vec3_t to_b = vec3_sub(triangle_vertices[1], origin);
	vec3_t to_c = vec3_sub(triangle_vertices[2], origin);
	vec3_t normal = vec3_cross(to_b, to_c);
	return vec3_normalized(normal);
}

float get_min(float x, float y) {
	return x < y ? x : y;
}

float get_max(float x, float y) {
	return x > y ? x : y;
}


void vertex_swap(vertex_projected* a, vertex_projected* b) {
	vertex_projected temp = *a;
	*a = *b;
	*b = temp;
}

void int_swap(int* a, int* b) {
	int c = *a;
	*a = *b;
	*b = c;
}

void float_swap(float* a, float* b) {
	float c = *a;
	*a = *b;
	*b = c;
}

void draw_flat_bottom2(vec2_int vertex, vec2_int edge1, vec2_int edge2, uint32_t color) {
	float inv_slope_1 = (edge1.x - vertex.x) / (float)(edge1.y - vertex.y);
	float inv_slope_2 = (edge2.x - vertex.x) / (float)(edge2.y - vertex.y);

	float x_start = vertex.x;
	float x_end = vertex.x;
	for (int y = vertex.y; y <= edge1.y; y++)
	{
		draw_line(x_start, y, x_end, y, color);
		x_start += inv_slope_1;
		x_end += inv_slope_2;
	}
}


void draw_flat_top2(vec2_int vertex, vec2_int edge1, vec2_int edge2, uint32_t color) {
	float inv_slope_1 = (edge1.x - vertex.x) / (float)(edge1.y - vertex.y);
	float inv_slope_2 = (edge2.x - vertex.x) / (float)(edge2.y - vertex.y);

	float x_start = vertex.x;
	float x_end = vertex.x;
	for (int y = vertex.y; y >= edge1.y; y--)
	{
		draw_line(x_start, y, x_end, y, color);
		x_start -= inv_slope_1;
		x_end -= inv_slope_2;
	}
}


void draw_flat_top(vec2_int min, vec2_int ver1, vec2_int ver2, uint32_t color) {
	vec2_int left, right;
	if (ver1.x < ver2.x) {
		left = ver1;
		right = ver2;
	}
	else {
		left = ver2;
		right = ver1;
	}

	int dy = abs(left.y - min.y);
	int leftDx = left.x - min.x;
	bool leftLine = leftDx == 0;

	float leftSlope = 0;
	if (!leftLine) {
		leftSlope = dy / (float)leftDx;
	}

	int rightDx = right.x - min.x;
	bool rightLine = rightDx == 0;

	float rightSlope = 0;
	if (!rightLine) {
		rightSlope = dy / (float)rightDx;
	}

	float leftX = min.x;
	float rightX = min.x;
	vec2_int min_pixel = { min.x, min.y };
	draw_pixel(min_pixel, color);
	for (int y = 0; y < dy; y++)
	{
		if (!leftLine) {
			leftX += 1 / leftSlope;
		}

		if (!rightLine) {
			rightX += 1 / rightSlope;
		}

		for (int x = round(leftX); x <= round(rightX); x++) {
			vec2_int position = { x,min.y - y };
			draw_pixel(position, color);
		}
	}

	for (size_t x = leftX; x < rightX; x++)
	{
		vec2_int position = { x,left.y  };
		draw_pixel(position, color);
	}
}

void draw_flat_bottom(vec2_int max, vec2_int ver1, vec2_int ver2, uint32_t color) {
	vec2_int left, right;
	if (ver1.x < ver2.x) {
		left = ver1;
		right = ver2;
	}
	else {
		left = ver2;
		right = ver1;
	}

	int dy = abs(left.y - max.y);

	int leftDx = left.x - max.x;
	bool leftLine = leftDx == 0;

	float leftSlope = 0;
	if (!leftLine) {
		leftSlope = dy / (float)leftDx;
	}

	int rightDx = right.x - max.x;
	bool rightLine = rightDx == 0;

	float rightSlope = 0;
	if (!rightLine) {
		rightSlope = dy / (float)rightDx;
	}

	float leftX = max.x;
	float rightX = max.x;
	vec2_int max_pixel = { max.x, max.y };
	draw_pixel(max_pixel, color);
	for (int y = 0; y < dy; y++)
	{
		if (!leftLine) {
			leftX += 1 / leftSlope;
		}

		if (!rightLine) {
			rightX += 1 / rightSlope;
		}

		for (int x = round(leftX); x <= round(rightX); x++) {
			vec2_int position = { x,y + max.y };
			draw_pixel(position, color);
		}
	}

	for (size_t x = leftX; x < rightX; x++)
	{
		vec2_int position = { x,left.y};
		draw_pixel(position, color);
	}
}

void draw_flat(vec2_int vertice, vec2_int edge1, vec2_int edge2, uint32_t color) {
	if (vertice.y > edge1.y) {
		draw_flat_top(vertice, edge1, edge2, color);
	}
	else {
		draw_flat_bottom(vertice, edge1, edge2, color);
	}
}


void draw_filled_triangle(vertex_projected vertex0, vertex_projected vertex1, vertex_projected vertex2, 
	uint32_t color, float intensity) {
	if (vertex0.y > vertex1.y) {
		vertex_swap(&vertex0, &vertex1);
	}

	if (vertex1.y > vertex2.y) {
		vertex_swap(&vertex1, &vertex2);
	}

	if (vertex0.y > vertex1.y) {
		vertex_swap(&vertex0, &vertex1);
	}

	float inv_slope_1 = 0;
	if (vertex1.y - vertex0.y != 0)
		inv_slope_1 = (float)(vertex1.x - vertex0.x) / abs(vertex1.y - vertex0.y);

	float inv_slope_2 = 0;
	if (vertex2.y - vertex0.y != 0)
		inv_slope_2 = (float)(vertex2.x - vertex0.x) / abs(vertex2.y - vertex0.y);

	for (int y = vertex0.y; y <= vertex1.y; y++)
	{
		int x_start = vertex1.x + (y - vertex1.y) * inv_slope_1;
		int x_end = vertex0.x + (y - vertex0.y) * inv_slope_2;

		if (x_start > x_end) {
			int_swap(&x_start, &x_end);
		}

		for (int x = x_start; x < x_end; x++)
		{
			draw_pixel2(x, y, vertex0, vertex1, vertex2, color, intensity);
		}
	}

	inv_slope_1 = 0;
	if (vertex2.y - vertex1.y != 0)
		inv_slope_1 = (float)(vertex2.x - vertex1.x) / abs(vertex2.y - vertex1.y);

	inv_slope_2 = 0;
	if (vertex2.y - vertex0.y != 0)
		inv_slope_2 = (float)(vertex2.x - vertex0.x) / abs(vertex2.y - vertex0.y);

	for (int y = vertex1.y; y <= vertex2.y; y++)
	{
		int x_start = vertex1.x + (y - vertex1.y) * inv_slope_1;
		int x_end = vertex0.x + (y - vertex0.y) * inv_slope_2;

		if (x_start > x_end) {
			int_swap(&x_start, &x_end);
		}

		for (int x = x_start; x < x_end; x++)
		{
			draw_pixel2(x, y, vertex0, vertex1, vertex2, color, intensity);
		}
	}
}


void draw_textured_triangle(vertex_projected vertex0 , float u0, float v0,
	vertex_projected vertex1, float u1, float v1,
	vertex_projected vertex2, float u2, float v2,
	uint32_t* texture, float intensity) {

	if (vertex0.y > vertex1.y) {
		vertex_swap(&vertex0, &vertex1);

		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	if (vertex1.y > vertex2.y) {
		vertex_swap(&vertex1, &vertex2);

		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}

	if (vertex0.y > vertex1.y) {
		vertex_swap(&vertex0, &vertex1);

		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	v0 = 1 - v0;
	v1 = 1 - v1;
	v2 = 1 - v2;

	tex2_t a_uv = { u0,v0 };
	tex2_t b_uv = { u1,v1 };
	tex2_t c_uv = { u2,v2 };

	float inv_slope_1 = 0; 
	if (vertex1.y - vertex0.y != 0)
		inv_slope_1 = (float)(vertex1.x - vertex0.x) / abs(vertex1.y - vertex0.y);

	float inv_slope_2 = 0;
	if (vertex2.y - vertex0.y != 0)
		inv_slope_2 = (float)(vertex2.x - vertex0.x) / abs(vertex2.y -vertex0.y);

	for (int y = vertex0.y; y <= vertex1.y; y++)
	{
		int x_start = vertex1.x + (y - vertex1.y) * inv_slope_1;
		int x_end = vertex0.x + (y - vertex0.y) * inv_slope_2;

		if (x_start > x_end) {
			int_swap(&x_start, &x_end);
		}

		for (int x = x_start; x < x_end; x++)
		{
			draw_texel(x, y, vertex0, vertex1, vertex2, a_uv, b_uv, c_uv, texture, intensity);
		}
	}

	inv_slope_1 = 0;
	if (vertex2.y - vertex1.y != 0)
		inv_slope_1 = (float)(vertex2.x - vertex1.x) / abs(vertex2.y - vertex1.y);

	inv_slope_2 = 0;
	if (vertex2.y - vertex0.y != 0)
		inv_slope_2 = (float)(vertex2.x - vertex0.x) / abs(vertex2.y -  vertex0.y);

	for (int y = vertex1.y; y <= vertex2.y; y++)
	{
		int x_start = vertex1.x + (y - vertex1.y) * inv_slope_1;
		int x_end = vertex0.x + (y - vertex0.y) * inv_slope_2;

		if (x_start > x_end) {
			int_swap(&x_start, &x_end);
		}

		for (int x = x_start; x < x_end; x++)
		{
			draw_texel(x, y, vertex0, vertex1, vertex2, a_uv, b_uv, c_uv, texture, intensity);
		}
	}
}

void draw_pixel2(int x, int y, vertex_projected a, vertex_projected b, vertex_projected c, uint32_t color, float intensity) {
	vec2_t p = { x, y };
	vec2_t point_a = vec2_from_projected(a);
	vec2_t point_b = vec2_from_projected(b);
	vec2_t point_c = vec2_from_projected(c);
	vec3_t weights = barycentric_weights(point_a, point_b, point_c, p);

	float reciprocal_w = 1 / a.w * weights.x + 1 / b.w * weights.y + 1 / c.w * weights.z;

	reciprocal_w = 1 - reciprocal_w;
	if (z_buffer[x + y * window_width] > reciprocal_w) {
		z_buffer[x + y * window_width] = reciprocal_w;
		vec2_int p1 = { x,y };
		draw_pixel(p1, light_apply_intensity(color, intensity));
	}
}

void draw_texel(int x, int y, vertex_projected a, vertex_projected b, vertex_projected c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv, uint32_t* texture, float intensity) {
	vec2_t p = { x,y };
	vec2_t point_a = vec2_from_projected(a);
	vec2_t point_b = vec2_from_projected(b);
	vec2_t point_c = vec2_from_projected(c);
	vec3_t weights = barycentric_weights(point_a, point_b, point_c, p);

	float u;
	float v;
	float reciprocal_w;

	u = a_uv.u / a.w * weights.x + b_uv.u / b.w * weights.y + c_uv.u / c.w * weights.z;
	v = a_uv.v / a.w * weights.x + b_uv.v / b.w * weights.y + c_uv.v / c.w * weights.z;

	reciprocal_w = 1 / a.w * weights.x + 1 / b.w * weights.y + 1 / c.w * weights.z;

	u /= reciprocal_w;
	v /= reciprocal_w;
	reciprocal_w = 1 - reciprocal_w;
	if (z_buffer[x + y * window_width] > reciprocal_w) {
		z_buffer[x + y * window_width] = reciprocal_w;
		// Map the UV coordinate to the full texture width and height
		int tex_x = abs((int)(u * texture_width)) % texture_width;
		int tex_y = abs((int)(v * texture_height)) % texture_height;

		//int color_index = texture_width * (int)(u * texture_height) + (v * texture_width);
		//if (color_index>=0 && color_index < texture_height * texture_width) {
		uint32_t color = texture[(texture_width * tex_y) + tex_x];
		vec2_int p1 = { x,y };
		draw_pixel(p1, light_apply_intensity(color, intensity));
		//}
	}
}


vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p) {
	vec2_t ab = vec2_sub(b, a);
	vec2_t bc = vec2_sub(c, b);
	vec2_t ac = vec2_sub(c, a);
	vec2_t ap = vec2_sub(p, a);
	vec2_t bp = vec2_sub(p, b);

	float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);
	float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;
	float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;
	float gamma = 1 - alpha - beta;

	vec3_t weights = { alpha, beta, gamma };
	return weights;
}