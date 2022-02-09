#ifndef BEACON_H
#define BEACON_H

struct beacon {
	int *arr;
	int x, y;
	int users;
};

struct beacon *beacon_new(int x, int y);
void beacon_detach(struct beacon *b);
void beacon_update(struct beacon *b);
void beacon_follow(struct beacon *b, int *x, int *y);
void beacon_delete(struct beacon *b);

void beacon_freeAll();
void beacon_updateAll();
struct beacon *beacon_searchAll(int x, int y);

#endif
