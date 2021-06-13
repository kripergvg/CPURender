#include <stdio.h>
#include "mesh.h"
#include "array.h"
#include <stdbool.h>
#include <stdlib.h>

//vec3_t cube_vertices[N_CUBE_VERTICES] = {
//	{.x = 0, .y = 0, .z = 0},
//	{.x = 1, .y = 1, .z = 0},
//	{.x = 1, .y = 0, .z = 0},
//
//	{.x = 0, .y = 0, .z = 1},
//	{.x = 1, .y = 1, .z = 1},
//	{.x = 1, .y = 0, .z = 1},
//};

vec3_t cube_vertices[N_CUBE_VERTICES]  = {
	{.x = -1, .y = -1, .z = -1 }, // 1
	{.x = -1, .y = 1, .z = -1 }, // 2
	{.x = 1, .y = 1, .z = -1 }, // 3
	{.x = 1, .y = -1, .z = -1 }, // 4
	{.x = 1, .y = 1, .z = 1 }, // 5
	{.x = 1, .y = -1, .z = 1 }, // 6
	{.x = -1, .y = 1, .z = 1 }, // 7
	{.x = -1, .y = -1, .z = 1 }  // 8
};

//face_t cube_faces[N_CUBE_FACES] = {
//	{.a = 1, .b = 2, .c = 3, .color = 0xFFFF0000 },
//	{.a = 4, .b = 5, .c = 6, .color = 0xFF00FF00 },
//	{.a = 1, .b = 2, .c = 4, .color = 0xFF0000ff },
//};

face_t cube_faces[N_CUBE_FACES] = {
	 {.a = 1, .b = 2, .c = 3, .a_uv = { 0, 1 }, .b_uv = { 0, 0 }, .c_uv = { 1, 0 },  .color = 0xFFFF0000, .uniq = 0 },
	{.a = 1, .b = 3, .c = 4, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFF0000, .uniq = 1 },

	{.a = 4, .b = 3, .c = 5, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFF00FF00  },
	{.a = 4, .b = 5, .c = 6, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1},.color = 0xFF00FF00  },

	{.a = 6, .b = 5, .c = 7, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFF0000ff  },
	{.a = 6, .b = 7, .c = 8, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFF0000ff  },

	{.a = 8, .b = 7, .c = 2, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFFFF00  },
	{.a = 8, .b = 2, .c = 1, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFFFF00  },

	{.a = 2, .b = 7, .c = 5, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFF00FF, .uniq = 3  },
	{.a = 2, .b = 5, .c = 3, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFF00FF, .uniq = 2  },

	{.a = 6, .b = 8, .c = 1, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFF00FFFF  },
	{.a = 6, .b = 1, .c = 4, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFF00FFFF  }
};

mesh_t mesh = {
	.vertices = NULL,
	.faces = NULL,
	.rotation = {0,0,0},
	.scale = {1,1,1},
	.translation = {0,0,9}
};

void load_cube(void) {
	for (size_t i = 0; i < N_CUBE_VERTICES; i++)
	{
		array_push(mesh.vertices, cube_vertices[i]);
	}

	for (size_t i = 0; i < N_CUBE_FACES; i++)
	{
		array_push(mesh.faces, cube_faces[i]);
	}
}

#define MAX_LINE 1000

char* find_whitespace(char* start) {
	while (*start != ' ')
	{
		start++;
	}

	return start;
}

bool load_obj(char* path) {
	FILE* file;
	fopen_s(&file, path, "r");
	if (file == NULL) {
		return false;
	}
	
	tex2_t* texcoords = NULL;

	char line[MAX_LINE];
	while (fgets(line, MAX_LINE, file) != NULL)
	{
		if (line[0] == 'v' && line[1] == ' ') {
			char* nextSymbol = line + 2;
			vec3_t vertex;

			vertex.x = strtof(nextSymbol, &nextSymbol);
			vertex.y = strtof(nextSymbol, &nextSymbol);
			vertex.z = strtof(nextSymbol, &nextSymbol);
			array_push(mesh.vertices, vertex);
		}
		else if (line[0] == 'v' && line[1] == 't') {
			tex2_t coords;
			sscanf(line, "vt %f %f", &coords.u, &coords.v);

			array_push(texcoords, coords);
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			int vertex_indices[3];
			int texture_indices[3];
			int normal_indices[3];
			sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &vertex_indices[0], &texture_indices[0], &normal_indices[0],
				&vertex_indices[1], &texture_indices[1], &normal_indices[1],
				&vertex_indices[2], &texture_indices[2], &normal_indices[2]);
			char* nextSymbol = line + 2;
			face_t face = {
				.a = vertex_indices[0] - 1,
				.b = vertex_indices[1] - 1,
				.c = vertex_indices[2] - 1,
				.color = 0xFFFFFFFF,
				.a_uv = texcoords[texture_indices[0] - 1],
				.b_uv = texcoords[texture_indices[1] - 1],
				.c_uv = texcoords[texture_indices[2] - 1],
			};
			array_push(mesh.faces, face);
		}
	}

	array_free(texcoords);
}

void unload_mesh(void) {
	array_free(mesh.faces);
	array_free(mesh.vertices);
}