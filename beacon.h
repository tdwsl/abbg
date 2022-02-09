#ifndef BEACON_H
#define BEACON_H

#include <stdbool.h>

struct beacon {
	int *arr;
	int x, y;
	int users;
	int dist;
};

struct beacon *beacon_new(int x, int y, int dist);
void beacon_detach(struct beacon *b);
void beacon_update(struct beacon *b);
void beacon_follow(struct beacon *b, int *x, int *y);
void beacon_delete(struct beacon *b);

void beacon_freeAll();
void beacon_updateAll();
struct beacon *beacon_searchAll(int x, int y, int dist);

#endif
