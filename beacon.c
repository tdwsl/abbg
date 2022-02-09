#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "beacon.h"
#include "util.h"

struct beacon *beacons = 0;
int beacon_num = 0;
int beacon_max = 0;

struct beacon *beacon_new(int x, int y, int dist) {
	if(map_outOfBounds(x, y))
		return 0;

	addObject(beacon);
	struct beacon *b = &beacons[beacon_num-1];

	b->x = x;
	b->y = y;
	b->users = 0;
	b->arr = malloc(sizeof(int)*map.w*map.h);
	b->dist = dist;
	beacon_update(b);

	return b;
}

void beacon_update(struct beacon *b) {
	for(int i = 0; i < map.w*map.h; i++) {
		int l = map_tileBlocks(map.arr[i]);
		b->arr[i] = (l == BLOCKS_SOLID || l == BLOCKS_WATER)*-1;
	}

	b->arr[b->y*map.w+b->x] = 1;

	heatmap(b->arr, map.w, map.h);

	for(int i = 0; i < b->dist; i++)
		for(int j = 0; j < map.w*map.h; j++) {
			if(b->arr[j] == 0)
				b->arr[j] = -2;
			else if(b->arr[j] != -1)
				b->arr[j]--;
		}
	for(int i = 0; i < map.w*map.h; i++)
		if(b->arr[i] <= -2)
			b->arr[i] = b->arr[i] * -1 - 1;

	/*move(0, 0);
	for(int i = 0; i < map.w*map.h; i++) {
		switch(b->arr[i]) {
		case -1:
			printw("#");
			break;
		case 0:
			printw("*");
			break;
		default:
			printw("%d", b->arr[i]%10);
			break;
		}
		if(!((i+1)%map.w))
			printw("\n");
	}
	while(getch() != '\n');*/
}

void beacon_follow(struct beacon *b, int *x, int *y) {
	if(b->arr[(*y)*map.w+(*x)] <= 0)
		return;

	for(int d = 0; d < 8; d++) {
		int dx, dy;
		if(d < 4) {
			dx = (*x) + dirs[d*2];
			dy = (*y) + dirs[d*2+1];
		}
		else {
			dx = (*x) + xddirs[(d-4)*2];
			dy = (*y) + xddirs[(d-4)*2+1];
		}

		if(map_outOfBounds(dx, dy))
			continue;

		if(b->arr[dy*map.w+dx] == b->arr[(*y)*map.w+(*x)]-1) {
			*x = dx;
			*y = dy;
			return;
		}
	}
}

void beacon_delete(struct beacon *b) {
	free(b->arr);
	b->arr = 0;
	deleteObject(b, beacon);
}

void beacon_detach(struct beacon *b) {
	b->users--;
	if(!b->users)
		beacon_delete(b);
}

struct beacon *beacon_searchAll(int x, int y, int dist) {
	for(int i = 0; i < beacon_num; i++)
		if(beacons[i].x == x && beacons[i].y == y && beacons[i].dist == dist)
			return &beacons[i];
	return 0;
}

void beacon_freeAll() {
	for(int i = 0; i < beacon_num; i++)
		free(beacons[i].arr);
	freeObjects(beacon);
}

void beacon_updateAll() {
	for(int i = 0; i < beacon_num; i++)
		beacon_update(&beacons[i]);
}
