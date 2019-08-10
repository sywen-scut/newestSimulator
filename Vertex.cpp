#include "Vertex.h"
#include "global.h"
#include <cstdlib>
using namespace std;
extern int netsize;
Vertex::Vertex(int length, int width) {
	w = width;
	l = length;
}

int Vertex::toIndex()
{
	return l*netsize  +w;
}

Vertex Vertex::operator-(Vertex v) {
	return Vertex(l - v.l, w - v.w);
}

bool Vertex::operator==(Vertex v) {
	return (v.l == l)  && (v.w == w);
}

int Vertex::Manhattan(Vertex v) {
	return  abs(w - v.w) + abs(l - v.l);
}

