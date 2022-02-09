#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <stdbool.h>
#include "map.h"
#include "colours.h"
#include "actor.h"
#include "beacon.h"
#include "item.h"
#include "designation.h"

static struct cursor {
	int x, y;
} cursor = {0, 0};

void generateMap() {
	map_init(60, 23, T_GRASS);

	map_scatter(T_TREE, 27);

	map_scatter(T_STONEWALL, 12);
	for(int i = 0; i < 10; i++)
		map_expand(T_STONEWALL);
}

void draw() {
	map_draw(0, 0);
	item_drawAll(0, 0);
	actor_drawAll(0, 0);
	designation_drawAll(0, 0);
	move(cursor.y, cursor.x);
}

void update() {
	actor_updateAll();
}

int main() {
	initscr();
	keypad(stdscr, 1);
	timeout(0);
	initColours();
	clear();

	srand(time(NULL));
	generateMap();

	int x, y;
	map_randomXY(&x, &y);
	struct actor *player = actor_new(x, y);

	bool quit = false;
	clock_t lastTime = clock();
	while(!quit) {
		draw();

		switch(getch()) {
		case 'Q':
			quit = true;
			break;
		case 'f':
			actor_moveTo(player, cursor.x, cursor.y);
			break;
		case '\n':
			map_designateDestroy(cursor.x, cursor.y);
			break;
		case KEY_UP:
			if(cursor.y > 0)
				cursor.y--;
			break;
		case KEY_DOWN:
			if(cursor.y < map.h-1)
				cursor.y++;
			break;
		case KEY_LEFT:
			if(cursor.x > 0)
				cursor.x--;
			break;
		case KEY_RIGHT:
			if(cursor.x < map.w-1)
				cursor.x++;
			break;
		}

		clock_t currentTime = clock();
		float diff = (float)(currentTime-lastTime)/CLOCKS_PER_SEC;

		if(diff > 0.1) {
			update();
			lastTime = currentTime;
		}
	}

	beacon_freeAll();
	actor_freeAll();
	map_free();
	designation_freeAll();
	item_freeAll();

	keypad(stdscr, 0);
	timeout(-1);
	endwin();
	return 0;
}
