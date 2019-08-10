#include <vector>
#include <queue>
#include <cmath>
#include "Vertex.h"
#include "general.h"
#include <iostream>

using namespace std;
class Task {
private:
	double executionVolume;
	vector<double> withNeibors;// >0, parent;
	
	int id;
	int applicationId;
	double ipc = 1;

public:
	Task(double ev, int aId, int tid);
	Task(double ev, int aId, int tid,vector<double>ns);
	double getExecutionVolume();
	int getApplicationId();
	int getId();
	double getIpc();
	double setIpc(double ipc);
	vector<double> getWithNeibors();

	void setWithNeibors(vector<double> mask);
	void bondingwith(int another,double dataSize);// type 0,child; type 1, parent;
};

class InvokedTask {
private:
	double executionVolume;
	
	vector<double> withNeibors;
	int id;
	int applicationId;
	double ipc;

	
	Vertex landlord; //mark the core 
	State state;
	vector<int> parents;
	vector<int> children;
	void recognize();
public:
	vector<double> dataProcessed;
	double executionVolumeFinished;
	vector<int> neibours;
	InvokedTask(Task task) {
		executionVolume = task.getExecutionVolume();
		withNeibors = task.getWithNeibors();
		state = unmapped;
		id = task.getId();
		applicationId = task.getApplicationId();
		ipc = task.getIpc();
		executionVolumeFinished = 0;
		state = unmapped;
		recognize();
		dataProcessed = vector<double>(withNeibors.size(), 0);
	}

	
	void nRound(double es, int nT);
	int predictExecutionTime(int execS);
	vector<double> unreceivedData();
	vector<double> untransferedData();
	vector<double> unprocessedData();
	//int unloadedData();
	void startRunning(Vertex v);
	void continueRunning(Vertex v);
	bool hasFinished();
	void release();
	void release2();
	State checkState();
	void setState(State);
	
	vector<double> getDataProcessed();
	Vertex getLandlord();
	int getApplicationId();
	vector<double> getWithNeibors();
	int getId();
	double getIpc();
	vector<int> getParents();
	vector<int> getChildren();
	double getExecutionVolume();
	double getExecutionVolumeFinished();
};