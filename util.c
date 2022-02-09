#include <time.h>
#include <stdbool.h>
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

void heatmap(int *arr, int w, int h) {
	bool stuck = false;
	for(int g = 1; !stuck; g++) {
		stuck = true;
		for(int i = 0; i < w*h; i++) {
			if(arr[i] != g)
				continue;

			for(int d = 0; d < 8; d++) {
				int dx, dy;
				if(d < 4) {
					dx = i%w+dirs[d*2];
					dy = i/w+dirs[d*2+1];
				}
				else {
					dx = i%w+xddirs[(d-4)*2];
					dy = i/w+xddirs[(d-4)*2+1];
				}

				if(dx < 0 || dy < 0 || dx >= w || dy >= h)
					continue;
				if(arr[dy*w+dx])
					continue;

				stuck = false;
				arr[dy*w+dx] = g+1;
			}
		}
	}

	for(int i = 0; i < w*h; i++) {
		if(arr[i] <= 0)
			arr[i] = -1;
		else
			arr[i]--;
	}
}
