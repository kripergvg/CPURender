#pragma once

#include "vector.h"
#include "triangle.h"
#include <stdbool.h>

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES (6*2)
//#define N_CUBE_FACES (1)
extern face_t cube_faces[N_CUBE_FACES];

typedef struct {
	vec3_t* vertices;
	face_t* faces;
	vec3_t rotation;
	vec3_t scale;
	vec3_t translation;
} mesh_t;

extern mesh_t mesh;

bool load_obj(char* path);
void load_cube(void);
void unload_mesh(void);