#ifndef ACTOR_H
#define ACTOR_H

#include <stdbool.h>
#include "beacon.h"
#include "designation.h"

struct actor {
	int x, y;
	struct beacon *beacon;
	struct designation *designation;
};

struct actor *actor_new(int x, int y);
void actor_delete(struct actor *a);
void actor_target(struct actor *a, int tx, int ty, int dist);
void actor_draw(struct actor *a, int x, int y);
void actor_moveTo(struct actor *a, int x, int y);
bool actor_idle(struct actor *a);

void actor_updateAll();
void actor_freeAll();
void actor_drawAll(int x, int y);

#endif
