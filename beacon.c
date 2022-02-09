#include <stdlib.h>
#include <stdbool.h>
#include "map.h"
#include "beacon.h"
#include "util.h"

struct beacon *beacons = 0;
int maxBeacons = 0;
int numBeacons = 0;

struct beacon *beacon_new(int x, int y) {
	if(map_outOfBounds(x, y))
		return 0;

	numBeacons++;
	if(numBeacons > maxBeacons) {
		maxBeacons = numBeacons;
		beacons = realloc(beacons, sizeof(struct beacon)*maxBeacons);
	}
	struct beacon *b = &beacons[numBeacons-1];

	b->x = x;
	b->y = y;
	b->users = 0;
	b->arr = malloc(sizeof(int)*map.w*map.h);
	beacon_update(b);

	return b;
}

void beacon_update(struct beacon *b) {
	for(int i = 0; i < map.w*map.h; i++) {
		int l = map_tileBlocks(map.arr[i]);
		b->arr[i] = (l == BLOCKS_SOLID || l == BLOCKS_WATER)*-1;
	}

	b->arr[b->y*map.w+b->x] = 1;

	bool stuck = false;
	for(int g = 1; !stuck; g++) {
		stuck = true;
		for(int i = 0; i < map.w*map.h; i++) {
			if(b->arr[i] != g)
				continue;

			for(int d = 0; d < 8; d++) {
				int dx, dy;
				if(d < 4) {
					dx = i%map.w+dirs[d*2];
					dy = i/map.w+dirs[d*2+1];
				}
				else {
					dx = i%map.w+xddirs[(d-4)*2];
					dy = i/map.w+xddirs[(d-4)*2+1];
				}

				if(map_outOfBounds(dx, dy))
					continue;
				if(b->arr[dy*map.w+dx])
					continue;

				stuck = false;
				b->arr[dy*map.w+dx] = g+1;
			}
		}
	}

	/*move(0, 0);
	for(int i = 0; i < map.w*map.h; i++) {
		switch(b->arr[i]) {
		case -1:
			printw("#");
			break;
		case 0:
			printw(" ");
			break;
		default:
			if(b->arr[i] < 15)
				printw(".");
			else
				printw("*");
			break;
		}
		if(!((i+1)%map.w))
			printw("\n");
	}
	while(getch() != '\n');*/
}

void beacon_follow(struct beacon *b, int *x, int *y) {
	if(!b->arr[(*y)*map.w+(*x)])
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
	int i;
	for(i = 0; &beacons[i] != b; i++);
	numBeacons--;
	beacons[i] = beacons[numBeacons];
}

void beacon_detach(struct beacon *b) {
	b->users--;
	if(!b->users)
		beacon_delete(b);
}

struct beacon *beacon_searchAll(int x, int y) {
	for(int i = 0; i < numBeacons; i++)
		if(beacons[i].x == x && beacons[i].y == y)
			return &beacons[numBeacons];
	return 0;
}

void beacon_freeAll() {
	for(int i = 0; i < numBeacons; i++)
		free(beacons[i].arr);
	numBeacons = 0;
	maxBeacons = 0;
	if(beacons)
		free(beacons);
	beacons = 0;
}

void beacon_updateAll() {
	for(int i = 0; i < numBeacons; i++)
		beacon_update(&beacons[i]);
}
