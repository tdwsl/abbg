#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "map.h"
#include "colours.h"
#include "util.h"

struct map map;

int baseTile = T_GRASS;

void map_init(int w, int h, int t) {
	map.w = w;
	map.h = h;
	map.arr = malloc(sizeof(int)*w*h);
	for(int i = 0; i < w*h; i++)
		map.arr[i] = t;
	baseTile = t;
}

int map_getTile(int x, int y) {
	if(map_outOfBounds(x, y))
		return T_OUT;
	else
		return map.arr[y*map.w+x];
}

void map_setTile(int x, int y, int t) {
	if(!map_outOfBounds(x, y))
		map.arr[y*map.w+x] = t;
}

void map_randomXY(int *x, int *y) {
	int xy = rand() % (map.w*map.h);
	while(map.arr[xy] != baseTile) {
		xy++;
		if(xy >= map.w*map.h)
			xy = 0;
	}
	*x = xy%map.w;
	*y = xy%map.h;
}

void map_scatter(int t, int n) {
	for(int i = 0; i < n; i++) {
		int xy = rand() % (map.w*map.h);
		while(++xy < map.w*map.h && map.arr[xy] != baseTile);
		if(xy < map.w*map.h)
			map.arr[xy] = t;
	}
}

void map_expand(int t) {
	for(int i = 0; i < map.w*map.h; i++) {
		if(map.arr[i] != t)
			continue;
		int d = rand()%4;
		int tx = i%map.w+dirs[d*2], ty = i/map.w+dirs[d*2+1];
		if(map_getTile(tx, ty) == baseTile)
			map_setTile(tx, ty, T_OUT);
	}
	for(int i = 0; i < map.w*map.h; i++)
		if(map.arr[i] == T_OUT)
			map.arr[i] = t;
}

char map_autotileChar(int *t) {
	int xy = t-map.arr;
	int x = xy%map.w, y = xy/map.w;

	const char at[] = {
		0xCE, 0xBA, 0xCD, 0xC8, 0xBA, 0xBA, 0xC9, 0xCC,
		0xCD, 0xBC, 0xCD, 0xCA, 0xBB, 0xB9, 0xCB, 0xCE,
	};

	int ati = 0;
	const int pwrs[] = {1, 2, 4, 8};
	for(int i = 0; i < 4; i++)
		if(map_getTile(x+dirs[i*2], y+dirs[i*2+1]) == *t)
			ati |= pwrs[i];

	return at[ati];
}

char map_tileChar(int *t) {
	switch(*t) {
	case T_GRASS: return ',';
	case T_TREE: return 'O';
	case T_WOODWALL: return '-';//return map_autotileChar(t);
	case T_STONEWALL: return '~';//return 176;
	case T_FLOOR: return '.';
	default: return '?';
	}
}

int map_tilePair(int t) {
	switch(t) {
	case T_GRASS:
		return BLACK_GREEN;
	case T_WOODWALL:
		return RED_BLACK;
	case T_TREE:
		return BLACK_RED;
	case T_STONEWALL:
		return WHITE_BLACK;
	default:
		return 0;
	}
}

void map_print() {
	for(int i = 0; i < map.w*map.h; i++) {
		printf("%c", map_tileChar(&map.arr[i]));
		if(!((i+1)%map.w))
			printf("\n");
	}
}

void map_fillRect(int x, int y, int w, int h, int t) {
	int x2 = x+w, y2 = y+h;
	for(; y < y2; y++)
		for(int i = x; i < x2; i++)
			map_setTile(i, y, t);
}

void map_draw(int x, int y) {
	for(int i = 0; i < map.w*map.h; i++) {
		attrset(COLOR_PAIR(map_tilePair(map.arr[i])));
		mvaddch(i/map.w+y, i%map.w+x, map_tileChar(&map.arr[i]));
	}
	attrset(A_NORMAL);
}

int map_tileBlocks(int t) {
	if(t == T_STONEWALL || t == T_WOODWALL || t == T_TREE || t == T_OUT)
		return BLOCKS_SOLID;
	if(t == T_DOORCLOSED)
		return BLOCKS_DOOR;
	return 0;
	switch(t) {
	T_OUT:
	T_WOODWALL:
	T_STONEWALL:
	T_TREE:
		return BLOCKS_SOLID;
	T_DOORCLOSED:
		return BLOCKS_DOOR;
	default:
		return 0;
	}
}

int map_blocks(int x, int y) {
	return map_tileBlocks(map_getTile(x, y));
}
