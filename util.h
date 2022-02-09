#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <stdlib.h>

#define seconds() ((float)clock()/CLOCKS_PER_SEC)
#define deleteObject(O, T) { int i; for(i = 0; &T ## s[i] != O; i++);\
	T ## _num--; T ## s[i] = T ## s[T ## _num]; }
#define freeObjects(T) { if(T ## s) free(T ## s); T ## s = 0; T ## _num = 0; T ## _max = 0; }
#define addObject(T) { T ## _num++; if(T ## _num > T ## _max)\
	{ T ## _max = T ## _num; T ## s = realloc(T ## s, sizeof(struct T) * T ## _num); } }

extern const int dirs[];
extern const int ddirs[];
extern const int xddirs[];

void heatmap(int *arr, int w, int h);

#endif
