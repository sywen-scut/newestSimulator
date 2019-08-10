#include <string>
#include <stdlib.h>  
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "Core.h"

class Map
{
private:
	vector<Core> cores;
	int width;
	int length;
public:
	vector<Core> initCores(double Freq, int width, int length);

	Map(double runFreq, int width, int length);

	int getWidth();

	int getLength();
	
	Core* getSpecificCore(Vertex v) ;

	Core* getSpecificCore(int index) ;

	bool turnOnCore(InvokedTask task,Vertex v);

	bool turnOffCore(Vertex v);

	void showMap();


	void writePtrace(FILE* fp);

	void getTemperature(string path);
	
};