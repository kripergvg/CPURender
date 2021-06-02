#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"
#include <stdlib.h>
#include "matrix.h"
#include "light.h"
#include "texture.h"
#include "upng.h"

triangle_t* triangles_to_render = NULL;

vec3_t camera_position = { 0,0, 0 };
vec3_t cube_rotation = { .x = 0, .y = 0, .z = 0 };

#define MAX_TRIANGLES 100000

bool is_running = false;
int previous_frame_time = 0;
int triangles_count = 0;

mat4_t proj_matrix;

typedef enum ViewModeType {
	Wireframe = 1,
	Vertices = 2,
	Filled = 4,
	Textured = 8 
} ViewMode;


ViewMode view_mode =  Wireframe | Textured;
bool culling = true;

void setup(void) {

	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	color_buffer_texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);

	z_buffer = (float*)malloc(sizeof(float) * window_width * window_height);
	triangles_to_render = (triangle_t*)malloc(sizeof(triangle_t) * MAX_TRIANGLES);

	float xFov = 60 * M_PI / 180;
	float yFov = 60 * M_PI / 180;
	float aspect =  (float)window_height / window_width;
	float znear = 0.1;
	float zfar = 100;
	proj_matrix = mat4_perspective(xFov, yFov, aspect, znear, zfar);

	//mesh_texture = (uint32_t*)REDBRICK_TEXTURE;
	//texture_width = 64;
	//texture_height = 64;

	//load_cube();
	load_png_texture_data("./assets/drone.png");
	load_obj("./assets/drone.obj");
}

void process_input(void) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
			}
			else if (event.key.keysym.sym == SDLK_1) {
				view_mode = Wireframe | Vertices;
			}
			else if (event.key.keysym.sym == SDLK_2) {
				view_mode = Wireframe;
			}
			else if (event.key.keysym.sym == SDLK_3) {
				view_mode = Filled;
			}
			else if (event.key.keysym.sym == SDLK_4) {
				view_mode = Filled | Wireframe;
			}
			else if (event.key.keysym.sym == SDLK_5) {
				view_mode = Textured;
			}
			else if (event.key.keysym.sym == SDLK_6) {
				view_mode = Textured | Wireframe;
			}
			else if (event.key.keysym.sym == SDLK_c) {
				culling = true;
			}
			else if (event.key.keysym.sym == SDLK_d) {
				culling = false;
			}
			else if (event.key.keysym.sym == SDLK_w) {
				mesh.translation.z += 0.05;
			}
			else if (event.key.keysym.sym == SDLK_s) {
				mesh.translation.z -= 0.05;
			}
			break;

		default:
			break;
		}
	}
}

void update(void) {

	int time_to_wait = previous_frame_time + FRAME_TARGET_TIME - SDL_GetTicks();
	if (time_to_wait > 0) {
		SDL_Delay(time_to_wait);
	}

	previous_frame_time = SDL_GetTicks();
	
	mesh.rotation.y += 0.01;
	//mesh.rotation.x += 0.01;
	//mesh.translation.z = 5;
	//mesh.rotation.z += 0.01;

	//mesh.scale.x += 0.002;

	mat4_t scale_matrix = mat4_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t translation_matrix = mat4_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);
	mat4_t rotation_matrix = mat4_rotation(mesh.rotation.x, mesh.rotation.y, mesh.rotation.z);
	mat4_t transform_matrix =  mat4_mul(translation_matrix, mat4_mul(rotation_matrix, scale_matrix));
	
	for (size_t i = 0; i < array_length(mesh.faces); i++)
	{
		face_t mesh_face = mesh.faces[i];
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[mesh_face.a];
		face_vertices[1] = mesh.vertices[mesh_face.b];
		face_vertices[2] = mesh.vertices[mesh_face.c];

		triangle_t projected_triangle;
		projected_triangle.color = mesh_face.color;

		vec3_t transformed[3];
		for (size_t v_index = 0; v_index < 3; v_index++)
		{
			vec4_t vertex4 = vec3_to_vec4(face_vertices[v_index]);
			vec3_t transformed_vertex = vec4_to_vec3(mat4_mul_vec4(transform_matrix, vertex4));

			transformed[v_index] = transformed_vertex;	
		}

		projected_triangle.uniq = mesh_face.uniq;

		vec3_t to_camera = vec3_sub(camera_position, transformed[0]);
		vec3_t normal = get_normal(transformed);
		if (!culling || vec3_dot(to_camera, normal) > 0) {

			vec3_t normal_inverse = { -normal.x, -normal.y,-normal.z };
			float intensity = vec3_dot(vec3_normalized(light.direction), normal_inverse);
			if (intensity < 0)
				intensity = 0;
			projected_triangle.color = light_apply_intensity(projected_triangle.color, intensity);
			projected_triangle.intensity = intensity;
			projected_triangle.texcoords[0] = mesh_face.a_uv;
			projected_triangle.texcoords[1] = mesh_face.b_uv;
			projected_triangle.texcoords[2] = mesh_face.c_uv;

			for (size_t v_index = 0; v_index < 3; v_index++)
			{
				vec4_t projected = mat4_mul_vec4_project(proj_matrix, vec3_to_vec4(transformed[v_index]));

				projected.x *= (window_width / 2.0);
				projected.y *= (window_height / 2.0);

				projected.y *= -1;

				projected.x += (window_width / 2.0);
				projected.y += (window_height / 2.0);

	
				vertex_projected tp = { projected.x, projected.y, projected.z, projected.w };

				projected_triangle.points[v_index] = tp;
			}
			triangles_to_render[triangles_count] = projected_triangle;
			triangles_count++;
			if (triangles_count > MAX_TRIANGLES) {

			}
		}
	}
}


void draw_test(void) {
	//vec2_int a = { 500,500 };
	//vec2_int b = { 700, 700 };
	//vec2_int c = { 400, 900 };
	//triangle_int triangle = { a,b,c };

	//draw_rectangle(a.x, a.y, 4, 4, 0xFF909090);
	//draw_rectangle(b.x, b.y, 4, 4, 0xFF909090);
	//draw_rectangle(c.x, c.y, 4, 4, 0xFF909090);
	////draw_triangle(triangle, 0xFF909090);
	//draw_filled_triangle(triangle, 0xFF00FF00);

	//vec2_int max = { 200, 100 };
	//vec2_int left = { 100, 300 };
	//vec2_int right = { 300, 300 };
	//vec2_int min = { 100,400 };

	//draw_flat_bottom(max, left, right, 0xFF00FF00);
	//draw_flat_top(min, left, right, 0xFF00FF00);
}

void draw_mesh(void) {
	vec2_int translation = { window_width / 2,window_height / 2 };

	for (size_t i = 0; i < triangles_count; i++)
	{
		triangle_t triangle = triangles_to_render[i];



		vec2_int v1 = { triangle.points[0].x, triangle.points[0].y };
		vec2_int v2 = { triangle.points[1].x, triangle.points[1].y };
		vec2_int v3 = { triangle.points[2].x, triangle.points[2].y };

		triangle_int triangle_int = { v1,v2,v3 };
		if ((view_mode & Filled) != 0) {
			draw_filled_triangle(triangle.points[0],
				triangle.points[1], 
				triangle.points[2], 
				triangle.color,
				triangle.intensity);
		}

		if ((view_mode & Textured) != 0) {
			draw_textured_triangle(triangle.points[0], triangle.texcoords[0].u, triangle.texcoords[0].v,
				triangle.points[1], triangle.texcoords[1].u, triangle.texcoords[1].v,
				triangle.points[2], triangle.texcoords[2].u, triangle.texcoords[2].v,
				mesh_texture, triangle.intensity);
		}
		
		if ((view_mode & Wireframe) != 0) {
			draw_triangle(triangle, 0xFFfc0345);
		}

		if ((view_mode & Vertices) != 0) {
			draw_rectangle(triangle.points[0].x, triangle.points[0].y, 3, 3, 0xFF03f8fc);
			draw_rectangle(triangle.points[1].x, triangle.points[1].y, 3, 3, 0xFF03f8fc);
			draw_rectangle(triangle.points[2].x, triangle.points[2].y, 3, 3, 0xFF03f8fc);
		}
	}
}

void render(void) {
	//draw_grid(0xFF999999); 
	draw_mesh();
	triangles_count = 0;
	render_color_buffer();
	clear_color_buffer(0xFF000000);
	clear_z_buffer();

	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
	is_running = initialize_window();
	if (!is_running)
		return 1;

	setup();

	while (is_running)
	{
		process_input();
		update();
		render();
	}

	array_free(triangles_to_render);
	unload_mesh();
	unload_texture();
	destroy_wibndow();

	return 0;
}