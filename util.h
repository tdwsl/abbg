#ifndef UTIL_H
#define UTIL_H

#include <time.h>

extern const int dirs[];
extern const int ddirs[];
extern const int xddirs[];

#define seconds() ((float)clock()/CLOCKS_PER_SEC)

#endif
