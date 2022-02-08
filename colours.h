#ifndef COLOURS_H
#define COLOURS_H

#include <ncurses.h>

#define initPair(A, B) init_pair(A ## B, COLOR_ ##A, COLOR_ ##B)

enum {
	BLACK_GREEN = 1,
	RED_BLACK,
};

#define initColours()\
	start_color();\
	initPair(BLACK, GREEN);\
	initPair(BROWN, BLACK);

#endif
