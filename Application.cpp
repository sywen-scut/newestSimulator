#include <cmath>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include "Application.h"
#include "include/rapidjson/reader.h"
using namespace rapidjson;
using namespace std;
vector<Task> generateTasks(int id,int taskVol, int nT) {
	vector<Task> ttasks;
	vector<double> mask(nT, 0);
	for (int i = 0; i < nT; i++) {
		Task task(taskVol, id, ttasks.size());
		task.setWithNeibors(mask);
		ttasks.push_back(task);
	}
	// sleep(1);
	// srand((unsigned)time(0));
	// int last_id=nT-1;
	// for(int i = 0; i < floor(0.25*nT); i++)
	// {
	// 	int temp_id = rand() % last_id +1 ;
	// 	ttasks[temp_id].setIpc(1.2);
	// }
	// for(int i = 0; i < floor(0.25*nT); i++)
	// {
	// 	int temp_id = rand() % last_id +1 ;
	// 	ttasks[temp_id].setIpc(1.5);
	// }
	
	return ttasks;
}

vector<Task> buildLine(vector<Task> tasks, int numTasks, double CCR,int fixedData) {
	for (vector<Task>::iterator iter = (tasks.begin() + 1); iter != tasks.end(); iter++) {
		double data = CCR * (*(iter - 1)).getExecutionVolume() + fixedData;
		(*(iter-1)).bondingwith((*iter).getId(),-data);
		(*iter).bondingwith((*(iter-1)).getId(),data);
	}
	return tasks;
}

vector<Task> buildStar(vector<Task> tasks, int numTasks, double CCR, int fixedData) {
	for (vector<Task>::iterator iter = (tasks.begin() + 1); iter != tasks.end(); iter++) {
		double data = CCR * (*tasks.begin()).getExecutionVolume() + fixedData;
		(*tasks.begin()).bondingwith((*iter).getId(),-data);
		(*iter).bondingwith((*tasks.begin()).getId(),data);
	}
	return tasks;
}

vector<Task> buildRandom(vector<Task> tasks, int numTasks, double CCR, int numEdges, string edgefile) {
	// for (vector<Task>::iterator iter = (tasks.begin() + 1); iter != tasks.end(); iter++) {
	// 	double data = CCR * (*tasks.begin()).getExecutionVolume() + fixedData;
	// 	(*tasks.begin()).bondingwith((*iter).getId(),-data);
	// 	(*iter).bondingwith((*tasks.begin()).getId(),data);
	// }
	double fixrate = CCR*5; //default edge volume is 100.
    ifstream file(edgefile);
	file >> numTasks;
	file >> numEdges;
	while(!file.eof()){
		int sid, did, commvol;
		file >> sid;
		file >> did;
		file >> commvol;
		tasks[sid-1].bondingwith(did-1,fixrate*commvol*(-1));
		tasks[did-1].bondingwith(sid-1,fixrate*commvol);
	}
	file.close();
	return tasks;
}


void Application::buildTaskGraph()
{
	for (auto t : tasks) {
		taskGraph.push_back(t.getWithNeibors());
	}
}

Application::Application(int nT,double r, int aT, int i,int taskVol,int type,int fixedData) {
	CCR = r;
	numTasks = nT;
	arrivalTime = aT;
	id = i;

	if (type == 0) {
		tasks = buildStar(generateTasks(id, taskVol, numTasks), numTasks, CCR, fixedData);
	}else{
			tasks = buildLine(generateTasks(id, taskVol, numTasks), numTasks, CCR, fixedData);
	}
	buildTaskGraph();
}

Application::Application(double ccr, int aT, int i,int taskVol,string edgefile){
    CCR =ccr;
	id = i;
	arrivalTime = aT;
	ifstream file(edgefile);
	file >> numTasks;
	file >> numEdges;
	file.close();
	tasks = buildRandom(generateTasks(id, taskVol, numTasks), numTasks, CCR, numEdges, edgefile);
	buildTaskGraph();

}

Application::Application(string filename)
{
  cout<<filename<<endl;
        ifstream file(filename);
	file >> id;
	file >> arrivalTime;
	file >> numTasks;
	for (int i = 0; i < numTasks; i++) {
		int tid; 
		double ev;
		file >> tid;
		file >> ev;
		ev =ev/10.0;
		Task task(ev, id, tid);
		vector<double> withNeibors;
		for (int j = 0; j < numTasks; j++) {
			double cv;
			file >> cv;
			if(cv == -0)
				cv = 0;
			if(cv < 0&&cv>-0.1)
				cv = -0.1;
			if(cv > 0&&cv<0.1)
				cv = 0.1;
			if(cv > 0)
				cv = ceil(cv);
			if(cv<0)
				cv = floor(cv);
			
			if(j>tid&&j!=0)
				cv = (-1)*fabs(cv);
			if(j<tid)
				cv = fabs(cv);
			if(j == tid)
				cv = 0;
			cv = cv/10.0;
			withNeibors.push_back(cv);
		}
		task.setWithNeibors(withNeibors);
		tasks.push_back(task);
	}
	buildTaskGraph();
}

Application::Application(int i, int aT, int nT, vector<Task> ts, vector<vector<double>> cvs)
{
	id = i;
	arrivalTime = aT;
	numTasks = nT;
	taskGraph = cvs;
	tasks = ts;
}

int Application::getArrivalTime() {
	return arrivalTime;
}

int Application::getNumTasks() {
	return  numTasks;
}
int Application::getNumEdges(){
	return numEdges;
}

vector<Task> Application::getTasks() {
	return tasks;
}

int Application::getId() {
	return id;
}

double Application::getCCR(){
	return CCR;
}

vector<vector<double>> Application::getTaskGraph()
{
	return taskGraph;
}

void Application::saveAsFile(string filenamePrefix, int prefix)
{
	
	filenamePrefix = filenamePrefix.append(to_string(id + prefix)+".txt");
	ofstream file(filenamePrefix,fstream::out);
	if (file)
		cout << " new file created" << endl; 
	else
		cout<<"created failed"<<endl;

	file << id << endl;
	file << arrivalTime << endl;
	file << numTasks << endl;
	
	for (auto i = tasks.begin(); i != tasks.end();i++) {
		Task task = *i;
		file << task.getId() << "\t" << task.getExecutionVolume() << "\t";
		vector<double> withNeibors = task.getWithNeibors();
		for (auto j = withNeibors.begin(); j != withNeibors.end(); j++) {
			file << *j << "\t";
		}
		file << endl;
	}
	file.close();
}

