#ifndef _CTILE_H
#define _CTILE_H

#include <ctype.h>

int CTILE_INTANGIBLE;
int CTILE_SOLID;

typedef union {
	uint8_t val;
	struct {
		uint8_t left_y  : 4;
		uint8_t right_y : 4;
	} slope;
} ctile_attributes;

typedef struct {
	uint8_t tile;
	uint8_t type;
	ctile_attributes atts;
} ctile_tile;

typedef struct {
	uint16_t width;
	uint16_t height;
	uint8_t page;
	void* metadata;
	ctile_tile* data;
} ctile_map;

ctile_map* ctile_load(char* filename);
ctile_tile* ctile_get_tile(ctile_map* map, int x, int y);
void ctile_draw(ALLEGRO_BITMAP* bmp, ctile_map* map, int twidth, int theight, float x, float y);
void ctile_destroy(ctile_map* map);

typedef struct {
	ctile_tile tile;
	int x;
	int y;
} ctile_collision_info;

ctile_collision_info* ctile_collision(ctile_map* map, float goal_x, float goal_y, float w, float h, int tw, int th, float* final_x, float* final_y, int* count);
#endif
