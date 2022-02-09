#include <time.h>
#include "util.h"

const int dirs[] = {
	0, -1,
	1, 0,
	0, 1,
	-1, 0,
};

const int ddirs[] = {
	0, -1,
	1,-1,
	1, 0,
	1,1,
	0, 1,
	-1,1,
	-1, 0,
	-1,-1,
};

const int xddirs[] = {
	1, -1,
	1, 1,
	-1, 1,
	-1, -1,
};
