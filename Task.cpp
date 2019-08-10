#include "Task.h"
using namespace std;
Task::Task(double ev,int aId,int tid) {
	id = tid;
	executionVolume = ev;
	applicationId = aId;
}

Task::Task(double ev, int aId, int tid, vector<double> ns)
{
	executionVolume = ev;
	id = tid;
	applicationId = aId;
	withNeibors = ns;
}

double Task::getExecutionVolume() {
	return executionVolume;
}
int Task::getApplicationId() {
	return applicationId;
}

int Task::getId() {
	return id;
}

double Task::getIpc(){
	return ipc;
}

double Task::setIpc(double ipc){
	this->ipc = ipc;
}

vector<double> Task::getWithNeibors()
{
	return withNeibors;
}

void Task::setWithNeibors(vector<double> mask)
{
	withNeibors = mask;
}

void Task::bondingwith(int another,double dataSize){
	withNeibors[another] = dataSize;
	//withNeibors.push_back(dataSize);
}

// class InvokedTask
void InvokedTask::recognize()
{
	int numT = withNeibors.size();
	for (int i = 0; i < numT; i++) {
		if (withNeibors[i] > 0) {
			parents.push_back(i);
			neibours.push_back(i);
		}
		else if (withNeibors[i] < 0) {
			children.push_back(i);
			neibours.push_back(i);
		}
	}
}

void InvokedTask::nRound(double es, int nT) {
	if (executionVolumeFinished > executionVolume) {
		executionVolumeFinished = executionVolume;
		state = tranfering;
	}
	bool tick = true;
	switch (state)
	{
	case waiting:
		for (int i = 0; i < nT; i++) {
			if (withNeibors[i] != dataProcessed[i]) {
				tick = false;
			}
		}
		if (tick) {
			state = executing;
		}
		break;
	case receiving:
		for (int i = 0; i < nT; i++) {
			if (withNeibors[i]>0)
			{
			  if (dataProcessed[i] < withNeibors[i])
				tick = false;
			}
		}
		if (tick) {
			state = executing;
		}
		break;
	case executing:
		if (executionVolumeFinished >= executionVolume) {
			/*parent start to send packets*/
			state = tranfering;
		}
		break;
	case tranfering:
		for (int i = 0; i < nT; i++) {
			if (withNeibors[i]<0){
			if (fabs(dataProcessed[i]) < fabs(withNeibors[i])) {
				tick = false;
			}
		}
		
		}
		if (tick) {
			state = finished;
		}
		break;
	default:
		break;
	}
}

void InvokedTask::startRunning(Vertex v) {
	landlord = v;
	state = waiting;
}

void InvokedTask::continueRunning(Vertex v) {
	landlord = v;
}

int InvokedTask::predictExecutionTime(int execS)
{
	int executionTime = ceil((executionVolume - executionVolumeFinished) / double(execS));
	return executionTime;
}

vector<double> InvokedTask::unreceivedData()
{
	vector<double> results;
	int numT = withNeibors.size();
	for (int i = 0; i < numT; i++) {
		if (withNeibors[i] > 0) {
			results.push_back(withNeibors[i] - dataProcessed[i]);
		}
		else {
			results.push_back(0);
		}
	}
	return results;
}

vector<double> InvokedTask::untransferedData()
{
	vector<double> results;
	int numT = withNeibors.size();
	for (int i = 0; i < numT; i++) {
		if (withNeibors[i] < 0) {
			results.push_back(withNeibors[i] - dataProcessed[i]);
		}
		else {
			results.push_back(0);
		}
	}
	return results;
}

vector<double> InvokedTask::unprocessedData()
{
	vector<double> results;
	int numT = withNeibors.size();
	for (int i = 0; i < numT; i++) {
		results.push_back(withNeibors[i] - dataProcessed[i]);
	}
	return results;
}

bool InvokedTask::hasFinished() {
	return state == 0;
}

void InvokedTask::release() {
	landlord = Vertex(-1, -1);
	if (state != finished) {
		state = unmapped;
	}
}
void InvokedTask::release2() {//for migration
	landlord = Vertex(-1, -1);

}
enum State InvokedTask::checkState()
{
	return state;
}

void InvokedTask::setState(State s)
{
	state = s;
}


vector<double> InvokedTask::getDataProcessed()
{
	return dataProcessed;
}

Vertex InvokedTask::getLandlord() {
	return landlord;
}

int InvokedTask::getApplicationId()
{
	return applicationId;
}

vector<double> InvokedTask::getWithNeibors()
{
	return withNeibors;
}

int InvokedTask::getId()
{
	return id;
}

double InvokedTask::getIpc(){
	return ipc;
}

vector<int> InvokedTask::getParents()
{
	return parents;
}

vector<int> InvokedTask::getChildren()
{
	return children;
}

double InvokedTask::getExecutionVolume()
{
	return executionVolume;
}

double InvokedTask::getExecutionVolumeFinished()
{
	return executionVolumeFinished;
}


