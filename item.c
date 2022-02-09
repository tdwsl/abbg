#include <ncurses.h>
#include "item.h"
#include "util.h"
#include "colours.h"

struct item *items = 0;
int item_num = 0;
int item_max = 0;

struct item *item_new(int x, int y, int type) {
	if(type == ITEM_NONE)
		return 0;

	addObject(item);
	struct item *t = &items[item_num-1];

	t->x = x;
	t->y = y;
	t->type = type;

	return t;
}

void item_delete(struct item *t) {
	deleteObject(t, item);
}

struct item *item_searchAll(int x, int y, int n) {
	for(int i = 0; i < item_num; i++)
		if(items[i].x == x && items[i].y == y) {
			n--;
			if(!n)
				return &items[i];
		}
	return 0;
}

int item_typePair(int t) {
	switch(t) {
	default:
		return BLACK_RED;
	}
}

char item_typeChar(int t) {
	switch(t) {
	case ITEM_WOOD:
		return '=';
	default:
		return '/';
	}
}

void item_draw(struct item *t, int x, int y) {
	int p = item_typePair(t->type);
	char c = item_typeChar(t->type);
	attron(COLOR_PAIR(p));
	mvaddch(t->y+y, t->x+x, c);
	attroff(COLOR_PAIR(p));
}

void item_drawAll(int x, int y) {
	for(int i = 0; i < item_num; i++)
		item_draw(&items[i], x, y);
}

void item_freeAll() {
	freeObjects(item);
}
