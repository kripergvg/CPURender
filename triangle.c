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


void draw_filled_triangle(triangle_int triangle, uint32_t color) {
	if (triangle.points[0].y == triangle.points[1].y && triangle.points[1].y == triangle.points[2].y) {
		int c = 10;
		// just a line
	}
	else if (triangle.points[0].y == triangle.points[1].y) {
		draw_flat(triangle.points[2], triangle.points[0], triangle.points[1], color);
	}
	else if (triangle.points[0].y == triangle.points[2].y) {
		draw_flat(triangle.points[1], triangle.points[0], triangle.points[2], color);
	}
	else if(triangle.points[1].y == triangle.points[2].y) {
		draw_flat(triangle.points[0], triangle.points[1], triangle.points[2], color);
	}
	else {
		int maxIndex = 0;
		vec2_int max = triangle.points[maxIndex];

		int minIndex = 1;
		vec2_int min = triangle.points[minIndex];

		if (triangle.points[1].y > max.y) {
			max = triangle.points[1];
			maxIndex = 1;
		}

		if (triangle.points[2].y > max.y) {
			max = triangle.points[2];
			maxIndex = 2;
		}

		if (triangle.points[0].y < min.y) {
			min = triangle.points[0];
			minIndex = 0;
		}

		if (triangle.points[2].y < min.y) {
			min = triangle.points[2];
			minIndex = 2;
		}

		int middleIndex = 2;
		vec2_int middle = triangle.points[middleIndex];
		for (size_t i = 0; i < 3; i++)
		{
			if (minIndex != i && maxIndex != i) {
				middleIndex = i;
				middle = triangle.points[middleIndex];
				break;
			}
		}

		int middlePointX = (float)(min.x - max.x) * (middle.y - max.y) / (float)(min.y - max.y) + max.x;
		vec2_int m = { middlePointX, middle.y };

		draw_flat_bottom2(min, middle, m, color);
		//draw_flat_bottom(min, middle, m, color);
		draw_flat_top2(max, middle, m, color);
	}
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

void draw_textured_triangle(int x0, int y0, float z0, float w0, float u0, float v0,
	int x1, int y1, float z1, float w1, float u1, float v1,
	int x2, int y2, float z2, float w2, float u2, float v2,
	uint32_t* texture, float intensity) {

	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);

		float_swap(&z0, &z1);
		float_swap(&w0, &w1);

		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);

		float_swap(&z1, &z2);
		float_swap(&w1, &w2);

		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}

	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);

		float_swap(&z0, &z1);
		float_swap(&w0, &w1);

		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	vec4_t point_a = { x0,y0,z0,w0 };
	vec4_t point_b = { x1,y1,z1,w1 };
	vec4_t point_c = { x2,y2,z2,w2 };
	tex2_t a_uv = { u0,v0 };
	tex2_t b_uv = { u1,v1 };
	tex2_t c_uv = { u2,v2 };

	float inv_slope_1 = 0; 
	if (y1 - y0 != 0)
		inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);

	float inv_slope_2 = 0;
	if (y2 - y0 != 0)
		inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	for (int y = y0; y <= y1; y++)
	{
		int x_start = x1 + (y - y1) * inv_slope_1;
		int x_end = x0 + (y - y0) * inv_slope_2;

		if (x_start > x_end) {
			int_swap(&x_start, &x_end);
		}

		for (int x = x_start; x < x_end; x++)
		{
			draw_texel(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture, intensity);
		}
	}

	inv_slope_1 = 0;
	if (y2 - y1 != 0)
		inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);

	inv_slope_2 = 0;
	if (y2 - y0 != 0)
		inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	for (int y = y1; y <= y2; y++)
	{
		int x_start = x1 + (y - y1) * inv_slope_1;
		int x_end = x0 + (y - y0) * inv_slope_2;

		if (x_start > x_end) {
			int_swap(&x_start, &x_end);
		}

		for (int x = x_start; x < x_end; x++)
		{
			draw_texel(x, y, point_a, point_b, point_c, a_uv, b_uv, c_uv, texture, intensity);
		}
	}
}

void draw_texel(int x, int y, vec4_t a, vec4_t b, vec4_t c, tex2_t a_uv, tex2_t b_uv, tex2_t c_uv, uint32_t* texture, float intensity) {
	vec2_t p = { x,y };
	vec2_t point_a = vec2_from_vec4(a);
	vec2_t point_b = vec2_from_vec4(b);
	vec2_t point_c = vec2_from_vec4(c);
	vec3_t weights = barycentric_weights(point_a, point_b, point_c, p);

	float u;
	float v;
	float reciprocal_w;

	u = a_uv.u/a.w * weights.x + b_uv.u /b.w * weights.y + c_uv.u/c.w * weights.z;
	v = a_uv.v/a.w * weights.x + b_uv.v /b.w * weights.y + c_uv.u/c.w * weights.z;

	reciprocal_w = 1 / a.w * weights.x + 1 / b.w * weights.y + 1 / c.w * weights.z;

	u /= reciprocal_w;
	v /= reciprocal_w;

	int color_index = texture_width * (int)(u * texture_height) + (v * texture_width);
	if (color_index>=0 && color_index < texture_height * texture_width) {
		uint32_t color = texture[color_index];
		vec2_int p1 = { x,y };
		if (intensity > 0.5) {
			int c = 10;
		}
		draw_pixel(p1, light_apply_intensity(color, intensity));
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