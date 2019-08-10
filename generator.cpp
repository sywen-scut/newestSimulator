#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>  
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include "Application.h"

int main(int argc, char const *argv[])
{
	int appNum = 12;
	int shape = 0;/*Line shape*/
	vector <Application> appSet;
	vector<string> edgefile;
	edgefile.push_back("../simulator/app/wen/Random/r9_1");
	edgefile.push_back("../simulator/app/wen/Random/r9_2");
	edgefile.push_back("../simulator/app/wen/Random/r9_3");
	edgefile.push_back("../simulator/app/wen/Random/r9_4");
	edgefile.push_back("../simulator/app/wen/Random/r12_1");
	edgefile.push_back("../simulator/app/wen/Random/r12_2");
	edgefile.push_back("../simulator/app/wen/Random/r12_3");
	edgefile.push_back("../simulator/app/wen/Random/r12_4");
	edgefile.push_back("../simulator/app/wen/Random/r16_1");
	edgefile.push_back("../simulator/app/wen/Random/r16_2");
	edgefile.push_back("../simulator/app/wen/Random/r16_3");
	edgefile.push_back("../simulator/app/wen/Random/r16_4");
	
	for (int i =0; i< appNum; i++)
	{
		// double ccr, int aT, int i,int taskVol,string edgefile
		//ccr 0.1-50, 0.2-100 ...
		Application temp_app(0.4, 0, i, 500, edgefile[i]);
		appSet.push_back(temp_app);
	}
	for (int i =0; i<appNum; i++)
	{
		appSet[i].saveAsFile("../simulator/app/wen/Random_TG200/app",0);
	}
	
	return 0;
}