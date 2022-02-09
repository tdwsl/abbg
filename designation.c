#include <ncurses.h>
#include "util.h"
#include "colours.h"
#include "designation.h"
#include "item.h"
#include "map.h"

struct designation *designations = 0;
int designation_num = 0;
int designation_max = 0;

struct designation *designation_new(int x, int y, int type, int tile, int item, int duration) {
	if(designation_searchAll(x, y))
		return 0;

	addObject(designation);
	struct designation *d = &designations[designation_num-1];

	d->x = x;
	d->y = y;
	d->type = type;
	d->tile = tile;
	d->item = item;
	d->duration = duration;
	d->active = false;

	return d;
}

void designation_delete(struct designation *d) {
	deleteObject(d, designation);
}

void designation_freeAll() {
	freeObjects(designation);
}

struct designation *designation_searchAll(int x, int y) {
	for(int i = 0; i < designation_num; i++) {
		struct designation *d = &designations[i];
		if(x == d->x && y == d->y)
			return d;
	}
	return 0;
}

void designation_complete(struct designation *d) {
	switch(d->type) {
	case DESIGNATION_DESTROY:
		map_setTile(d->x, d->y, d->tile);
		item_new(d->x, d->y, d->item);
		break;
	case DESIGNATION_PLACE:
		map_setTile(d->x, d->y, d->tile);
		break;
	}
}

int designation_typePair(int t) {
	switch(t) {
	case DESIGNATION_DESTROY:
		return BLACK_RED;
	case DESIGNATION_PLACE:
		return BLACK_GREEN;
	}
}

void designation_draw(struct designation *d, int x, int y) {
	float sec = seconds();
	if(sec - (int)sec > 0.3)
		return;

	int p = designation_typePair(d->type);
	attron(COLOR_PAIR(p));
	mvaddch(d->y+y, d->x+x, 'X');
	attroff(COLOR_PAIR(p));
}

void designation_drawAll(int x, int y) {
	for(int i = 0; i < designation_num; i++)
		designation_draw(&designations[i], x, y);
}

struct designation *designation_findNearest(int x, int y) {
	if(!designation_num)
		return 0;

	int *arr = malloc(sizeof(int)*map.w*map.h);
	for(int i = 0; i < map.w*map.h; i++) {
		int b = map_tileBlocks(map.arr[i]);
		arr[i] = (b == BLOCKS_SOLID || b == BLOCKS_WATER) * -1;
	}
	for(int i = 0; i < designation_num; i++)
		arr[designations[i].y*map.w+designations[i].x] = 0;
	arr[y*map.w+x] = 1;

	heatmap(arr, map.w, map.h);

	struct designation **des = malloc(sizeof(struct designation*)*designation_num);
	int num = 0;

	for(int i = 0; i < designation_num; i++) {
		struct designation *d = &designations[i];
		d->distance = arr[(d->y)*map.w+(d->x)];

		if(d->distance == -1 || d->active)
			continue;

		des[num] = d;
		num++;
	}

	free(arr);

	if(num == 0) {
		free(des);
		return 0;
	}

	struct designation *f;
	do {
		f = 0;
		for(int i = 1; i < num; i++)
			if(des[i]->distance < des[i-1]->distance) {
				f = des[i-1];
				des[i-1] = des[i];
				des[i] = f;
			}
	} while(f);

	struct designation *d = *des;
	free(des);

	return d;
}
