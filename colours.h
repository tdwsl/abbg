#ifndef COLOURS_H
#define COLOURS_H

#include <ncurses.h>

#define initPair(A, B) init_pair(A ## _ ## B, COLOR_ ## B, COLOR_ ## A)

enum {
	BLACK_GREEN = 1,
	RED_BLACK,
	BLACK_RED,
	WHITE_BLACK,
};

#define initColours()\
	start_color();\
	initPair(BLACK, GREEN);\
	initPair(RED, BLACK);\
	initPair(BLACK, RED);\
	initPair(WHITE, BLACK);

#endif
