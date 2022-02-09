#ifndef DESIGNATION_H
#define DESIGNATION_H

#include <stdbool.h>

enum {
	DESIGNATION_PLACE,
	DESIGNATION_DESTROY,
};

struct designation {
	int x, y;
	int type;
	int tile;
	int item;
	int duration;
	bool active;
	int distance;
};

struct designation *designation_new(int x, int y, int type, int tile, int item, int duration);
void designation_delete(struct designation *d);
void designation_draw(struct designation *d, int x, int y);
void designation_complete(struct designation *d);

struct designation *designation_searchAll(int x, int y);
void designation_drawAll(int x, int y);
void designation_freeAll();
struct designation *designation_findNearest(int x, int y);

#endif
