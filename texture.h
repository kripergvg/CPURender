#pragma once
#include <stdint.h>
#include "upng.h"

typedef struct {
    float u;
    float v;
} tex2_t;

extern int texture_width;
extern int texture_height;

extern const uint8_t REDBRICK_TEXTURE[];

extern uint32_t* mesh_texture;
extern upng_t* png_texture;

void load_png_texture_data(char* filename);
void unload_texture(void);


vec2_t vec2_from_tex2(tex2_t v);
tex2_t tex2_from_vec2(vec2_t v);