#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "map.h"
#include "beacon.h"
#include "actor.h"
#include "util.h"
#include "designation.h"

struct actor *actors = 0;
int actor_num = 0;
int actor_max = 0;

struct actor *actor_new(int x, int y) {
	addObject(actor);
	struct actor *a = &actors[actor_num-1];

	a->x = x;
	a->y = y;
	a->beacon = 0;
	a->designation = 0;

	return a;
}

void actor_detachBeacon(struct actor *a) {
	if(a->beacon) {
		beacon_detach(a->beacon);
		a->beacon = 0;
	}
}

void actor_delete(struct actor *a) {
	actor_detachBeacon(a);
	deleteObject(a, actor);
}

void actor_freeAll() {
	freeObjects(actor);
}

void actor_target(struct actor *a, int x, int y, int dist) {
	if(a->beacon)
		actor_detachBeacon(a);

	a->beacon = beacon_searchAll(x, y, dist);
	if(a->beacon) {
		a->beacon->users++;
		return;
	}

	a->beacon = beacon_new(x, y, dist);
	if(a->beacon) {
		a->beacon->users++;
		return;
	}
}

void actor_moveTo(struct actor *a, int x, int y) {
	if(map_blocks(x, y) == BLOCKS_DOOR) {
		map_setTile(x, y, T_DOOROPEN);
		return;
	}

	if(map_blocks(x, y))
		return;

	a->x = x;
	a->y = y;
}

void actor_stop(struct actor *a) {
	if(a->designation) {
		a->designation->active = false;
		a->designation = 0;
	}
	if(a->beacon)
		actor_detachBeacon(a);
}

void actor_findTask(struct actor *a) {
	struct designation *d = designation_findNearest(a->x, a->y);
	if(!d)
		return;

	if(d->active)
		return;

	actor_target(a, d->x, d->y, 1);
	if(!a->beacon)
		return;

	d->active = true;
	a->designation = d;
}

void actor_followBeacon(struct actor *a) {
	if(!a->beacon)
		return;

	int dx = a->x, dy = a->y;
	beacon_follow(a->beacon, &dx, &dy);

	actor_moveTo(a, dx, dy);
}

void actor_progressDesignation(struct actor *a) {
	a->designation->duration--;
	if(a->designation->duration <= 0) {
		designation_complete(a->designation);
		designation_delete(a->designation);
		a->designation = 0;
		if(a->beacon)
			actor_detachBeacon(a);
	}
}

void actor_update(struct actor *a) {
	if(actor_idle(a)) {
		actor_findTask(a);
		return;
	}

	if(a->designation) {
		if(a->beacon->arr[(a->y)*map.w+(a->x)])
			actor_followBeacon(a);
		else
			actor_progressDesignation(a);
	}
	else if(a->beacon) {
		actor_followBeacon(a);
		if(a->beacon->arr[(a->y)*map.w+(a->x)] <= 0)
			actor_detachBeacon(a);
	}
}

void actor_draw(struct actor *a, int x, int y) {
	mvaddch(a->y+y, a->x+x, '@');
}

void actor_updateAll() {
	for(int i = 0; i < actor_num; i++)
		actor_update(&actors[i]);
}

void actor_drawAll(int x, int y) {
	for(int i = 0; i < actor_num; i++)
		actor_draw(&actors[i], x, y);
}

bool actor_idle(struct actor *a) {
	if(a->beacon)
		return false;
	if(a->designation)
		return false;

	return true;
}
