#include "light.h"

light_t light = { .direction = {0,0, 1} };

uint32_t light_apply_intensity(uint32_t color, float intencity) {
	uint32_t a = color & 0xFF000000;
	uint32_t r = ((color >> (4 * 4)) & 0x000000FF)* intencity;
	uint32_t g = ((color >> (2 * 4)) & 0x000000FF)* intencity;
	uint32_t b = (color & 0x000000FF) * intencity;
	uint32_t result = a | (r << (4 * 4)) | (g << (2 * 4)) | b;
	return result;
}