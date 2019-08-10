#pragma once
#include "Vertex.h"

struct Region {
	Vertex start_p;
	int w;
	int l;
	Vertex end_p;
	int area;
	Region();
	Region(Vertex p, int w = -1, int l = -1);

};
