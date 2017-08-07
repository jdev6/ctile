#ifndef _CTILE_H
#define _CTILE_H

#include <ctype.h>

enum ctile_type {
	CTILE_NORMAL  = 0,
	CTILE_SLOPE   = 1,
	CTILE_SPAWNER = 2,
	CTILE_SPECIAL = 3
};

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
void ctile_draw(ALLEGRO_BITMAP* bmp, ctile_map* map, int twidth, int theight, float x, float y);
void ctile_destroy(ctile_map* map);

typedef struct {
	ctile_tile tile;
	int x;
	int y;
} ctile_collision_info;

ctile_collision_info* ctile_collision(ctile_map* map, float goal_x, float goal_y, float w, float h, int tw, int th, float* final_x, float* final_y, int* count);
#endif
