#include <allegro5/allegro5.h>
#include "ctile.h"

ctile_map* ctile_load(char* filename) {
	//allocate map
	ctile_map* map = al_malloc(sizeof(ctile_map));
	map->metadata = map->data = NULL;

	ALLEGRO_FILE* file = al_fopen(filename, "rb");
	if (!file) return NULL;
	
	//HEADER
	char name[6];
	al_fread(file, name, 5);
	name[5] = '\0';
	if (strcmp(name, "ctile") != 0) {
		al_fclose(file);
		return NULL;
	}
	
	map->width = al_fread16be(file);
	map->height = al_fread16be(file);
	
	map->page = (uint8_t) al_fgetc(file);

	map->metadata = al_malloc(16);
	al_fread(file, map->metadata, 16);

	//DATA
	size_t datalen = map->width * map->height * sizeof(ctile_tile);
	map->data = al_malloc(datalen);
	al_fread(file, (void*)map->data, datalen);

	al_fclose(file);
	return map;
}

ctile_tile* ctile_get_tile(ctile_map* map, int x, int y) {
	int i = x + (y * map->width);
	return i <= map->width*map->height ? &(map->data[i]) : NULL;
}

void ctile_draw(ALLEGRO_BITMAP* bmp, ctile_map* map, int twidth, int theight, float x, float y) {
	size_t len = map->width * map->height;
	for (int i = 0; i < len; i++) {
		ctile_tile tile = map->data[i];
		al_draw_bitmap_region(bmp, twidth * tile.tile, theight * map->page, twidth, theight, twidth*(i%map->width) + x, theight*(int)(i/map->width) + y, 0);
	}
}

void ctile_destroy(ctile_map* map) {
	if (map) {
		if (map->data) al_free(map->data);
		if (map->metadata) al_free(map->metadata);
		al_free(map);
	}
}

//COLLISION
inline int _AABB_COLLISION(float b1_x, float b1_y, float b1_w, float b1_h, float b2_x, float b2_y, float b2_w, float b2_h) {
	return !((b1_x > b2_x + b2_w - 1) ||
		     (b1_y > b2_y + b2_h - 1) ||
		     (b2_x > b1_x + b1_w - 1) ||
		     (b2_y > b1_y + b1_h - 1)); 
}

//returns Scollisons AND pùts count in arg count (ONLY returns conllisions if count IS n't null)
ctile_collision_info* ctile_collision(ctile_map* map, float goal_x, float goal_y, float w, float h, int tw, int th, float* x, float* y, int* count) {
	size_t len = map->width * map->height;
	ctile_collision_info* cols = NULL;
	int col_count = 0;
	
	//STEP X
	for (int i = 0; i < len; i++) {
		int tile_x = tw*(i%map->width), tile_y = th*(int)(i/map->width);
		if (map->data[i].tile > 0 && _AABB_COLLISION(goal_x,*y, w,h, tile_x,tile_y,tw,th)) {
			col_count++;
			//printf("coallision %f %f, %i %i\n", goal_x, goal_y, tile_x, tile_y);
		}
	}
	if (col_count == 0) {
		*x = goal_x;
	}
	
	col_count = 0;
	//STEP Y
	for (int i = 0; i < len; i++) {
		int tile_x = tw*(i%map->width), tile_y = th*(int)(i/map->width);
		if (map->data[i].tile > 0 && _AABB_COLLISION(*x,goal_y, w,h, tile_x,tile_y,tw,th)) {
			col_count++;
			//printf("coallision %f %f, %i %i\n", goal_x, goal_y, tile_x, tile_y);
		}
	}
	if (col_count == 0) {
		*y = goal_y;
	}
	return cols;
}
