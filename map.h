#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

enum {
	T_GRASS,
	T_STONEWALL,
	T_WOODWALL,
	T_FLOOR,
	T_TREE,
	T_OUT,
	T_NONE,
	T_DOOROPEN,
	T_DOORCLOSED,
};

enum {
	BLOCKS_DOOR=1,
	BLOCKS_SOLID=2,
	BLOCKS_WATER=3,
};

struct map {
	int w, h;
	int *arr;
};

extern struct map map;

void map_init(int w, int h, int t);
void map_setTile(int x, int y, int t);
int map_getTile(int x, int y);
#define map_free() free(map.arr)
void map_scatter(int t, int n);
#define map_get() map
#define map_set(M) map = M
void map_print();
#define map_outOfBounds(x, y) (x < 0 || y < 0 || x >= map.w || y >= map.h)
void map_fillRect(int x, int y, int w, int h, int t);
void map_expand(int t);
void map_draw(int x, int y);
int map_tileBlocks(int t);
int map_blocks(int x, int y);
void map_randomXY(int *x, int *y);

#endif
