#pragma once
#include <math.h>
struct Vertex {
	int w;
	int l;
	Vertex(int length = -1, int width = -1);
	int toIndex();
	Vertex operator-(Vertex v);
	bool operator==(Vertex v);
	int Manhattan(Vertex v);

	//static Vertex toVertex(int i, Map map);
};
