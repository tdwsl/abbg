#ifndef ACTOR_H
#define ACTOR_H

#include "beacon.h"

struct actor {
	int x, y;
	int tx, ty;
	struct beacon *beacon;
};

struct actor *actor_new(int x, int y);
void actor_delete(struct actor *a);
void actor_target(struct actor *a, int tx, int ty);
void actor_draw(struct actor *a, int x, int y);
void actor_moveTo(struct actor *a, int x, int y);

void actor_updateAll();
void actor_freeAll();
void actor_drawAll(int x, int y);

#endif
