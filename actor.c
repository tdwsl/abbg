#include <stdlib.h>
#include <ncurses.h>
#include "map.h"
#include "beacon.h"
#include "actor.h"

struct actor *actors = 0;
int numActors = 0;
int maxActors = 0;

struct actor *actor_new(int x, int y) {
	numActors++;
	if(numActors > maxActors) {
		maxActors = numActors;
		actors = realloc(actors, sizeof(struct actor)*maxActors);
	}
	struct actor *a = &actors[numActors-1];

	a->x = x;
	a->y = y;
	a->tx = x;
	a->ty = y;
	a->beacon = 0;

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
	int i;
	for(i = 0; &actors[i] != a; i++);
	numActors--;
	actors[i] = actors[numActors];
}

void actor_freeAll() {
	numActors = 0;
	maxActors = 0;
	if(actors)
		free(actors);
	actors = 0;
}

void actor_target(struct actor *a, int x, int y) {
	a->tx = x;
	a->ty = y;

	if(a->beacon)
		actor_detachBeacon(a);

	a->beacon = beacon_searchAll(x, y);
	if(a->beacon) {
		a->beacon->users++;
		return;
	}

	a->beacon = beacon_new(x, y);
	if(a->beacon) {
		a->beacon->users++;
		return;
	}

	/* failed */
	a->tx = a->x;
	a->ty = a->y;
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

void actor_followBeacon(struct actor *a) {
	if(!a->beacon)
		return;

	int dx = a->x, dy = a->y;
	beacon_follow(a->beacon, &dx, &dy);

	actor_moveTo(a, dx, dy);
	if(a->x == a->tx && a->y == a->ty)
		actor_detachBeacon(a);
}

void actor_update(struct actor *a) {
	if(a->x != a->tx || a->y != a->ty)
		actor_followBeacon(a);
}

void actor_draw(struct actor *a, int x, int y) {
	mvaddch(a->y+y, a->x+x, '@');
}

void actor_updateAll() {
	for(int i = 0; i < numActors; i++)
		actor_update(&actors[i]);
}

void actor_drawAll(int x, int y) {
	for(int i = 0; i < numActors; i++)
		actor_draw(&actors[i], x, y);
}
