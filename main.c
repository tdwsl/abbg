#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "map.h"

int main() {
	srand(time(NULL));
	map_init(60, 23, T_GRASS);

	initscr();
	initColours();
	clear();

	map_scatter(T_TREE, 27);

	map_scatter(T_STONEWALL, 12);
	for(int i = 0; i < 10; i++)
		map_expand(T_STONEWALL);

	map_fillRect(8, 4, 9, 5, T_WOODWALL);
	map_fillRect(9, 5, 7, 3, T_FLOOR);
	map_setTile(11, 8, T_FLOOR);

	map_draw(0, 0);
	getch();

	map_free();
	endwin();
	return 0;
}
