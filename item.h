#ifndef ITEM_H
#define ITEM_H

enum {
	ITEM_NONE,
	ITEM_WOOD,
	ITEM_STONE,
	ITEM_IMPOSSIBLE,
};

struct item {
	int type;
	int x, y;
};

void item_draw(struct item *t, int x, int y);
struct item *item_new(int x, int y, int type);
void item_delete(struct item *t);

struct item *item_searchAll(int x, int y, int n);
void item_freeAll();
void item_drawAll(int x, int y);

#endif
