#include "Region.h"

using namespace std;
Region::Region(){
	this->w = -1;
	this->l = -1;
}
Region::Region(Vertex p, int w, int  l) {
	this->start_p = p;
	this->w = w;
	this->l = l;
	Vertex end_piont(p.l+l-1, p.w + w-1);
	this->end_p = end_piont;
	this->area = w * l;
}

