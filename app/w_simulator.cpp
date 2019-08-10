#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>  
#include <algorithm>
#include <cmath>
#include <fstream>
#include "Clock.h"
#include "Application.h"
using namespace std;

class Core {
private:
	double Freq;
	int application_id;
	int task_id;
public:
	Vertex location(0,0,0);
	bool active;
	double p_power_leakage = 0.5; //default normalize
	double p_power_dynamic = 0;
	double r_power = 0;
	
	Core(double runFreq, int index){
		active = false;
		this->runFreq = runFreq;
		location.l = floor(index / 8);
		location.w = index% 8;

	}


	/*turn on the core running specific task*/
	void turnOn(int a_id, int t_id){
		applicationId = a_id;
		task_id = t_id;
		active = true;
		p_power_dynamic = runFreq;
	}

	/*turn off the core*/
	void turnOff(){
		applicationId = -1;
		task_id = -1;
		active = false;
		p_power_dynamic = 0;
	}

	void turnOn_router(){
		r_power = 0.3;
	}

	void turnOff_router(){
		r_power = 0;
	}

	// Vertex getCoordinate();
	int getApplicationId(){
		return application_id;
	}
	int getTaskId(){
		return task_id;
	}
	double getFreq(){
		return runFreq;
	}

}


class Map
{
private:
	vector<Core> cores;
	int width;
	int length;
public:
	vector<Core> initCores(double Freq, int width, int length){
		vector<Core> cores;
		for (int l = 0; l < length; l++) {
		
			for (int w = 0; w < width; w++) {
				Core core(Freq, w+ l * width);
				cores.push_back(core);
			}
		}
	
		return cores;
	}

	Map(double runFreq, int width, int length){
		cores = initCores(Freq, width, length);
	}
	int Map::getWidth() {
		return width;
	}

	int Map::getLength() {
		return length;
	}
	
	Core* Map::getSpecificCore(Vertex v) {
		int index = v.toIndex();
		return &(cores.at(index));
	}

	Core* Map::getSpecificCore(int index) {
		return &(cores.at(index));
	}

	bool turnOnCore(InvokedTask task,Vertex v){
		if (!getSpecificCore(v).active)
		{
			getSpecificCore(v)->turnOn(task.getApplicationId(), task.getId())
			return true;
		}
		return false;
	}

	bool turnOffCore(Vertex ver){
		if (getSpecificCore(v).active)
		{
			getSpecificCore(v)->turnOff();
			return true;
		}
		return false;

	}

	void showMap(){
		cout << endl;
		for(int k = 0; k < length; k++) {
				for (int j = 0; j < width; j++) {
					Vertex temp(k, j);
					cout << cores.at(temp.toIndex()).getTaskId() << " ";
				}
			cout << endl;
		}
		cout << endl;
	}
	// void writePtraceFile(string fn);
	// void writeFlpsFiles(string fn);
	
	
};

squareMapping(Map* map, vector<InvokedTask> tasks){
	int count = 0;
	while (1){
		for (int j =0; j < map->getLength(); j++) {
			for (int k =0; k < ceil(sqrt(map->tasks.size())); k++) {
				Vertex temp(j, k);
				if (!map->getSpecificCore(temp).active) {
					map->turnOnCore(tasks[count], temp);
					tasks.at(count).startRunning(temp);
					count++;
					if(count == tasks.size())
						break;
					//map one more task 	
				}
			}
		
		}
	}
	if (count == tasks.size()) {
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
		return false;
		}
	}
}	

chessMapping(Map* map,vector<InvokedTask> tasks){
	int count = 0;
	while (1){
		for (int j =0; j < map->getLength(); j+=2) {
			if (j%2 == 0){
				for (int k =0; k < 2*ceil(sqrt(map->tasks.size())); k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp).active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
						if(count == tasks.size())
							break;
						//map one more task 	
					}
				}
			}
			else{
				for (int k =1; k < 2*ceil(sqrt(map->tasks.size())); k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp).active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
						if(count == tasks.size())
							break;
						//map one more task 	
					}
				}
			}

		}
	}
	if (count == tasks.size()) {
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
		return false;
		}
	}

}

int variance(vector<int> a) {
	return *max_element(a.begin(), a.end()) - (*min_element(a.begin(), a.end()));
}
bool distanceCompare(vector<int>a, vector<int>b) {

	bool small = true;

	if (a.size() == 0 && b.size() == 0) {
		return false;
	}
	if (a.size() == 0) {
		return true;
	}
	if (b.size() == 0) {
		return false;
	}

	int sumA = std::accumulate(a.begin(), a.end(), 0);
	int sumB = std::accumulate(b.begin(), b.end(), 0);
	if (sumA > sumB) {
		return !small;
	}
	else {
		if (sumA < sumB) {
			return small;
		}
		else {

			int varianceA = variance(a);
			int varianceB = variance(b);
			if (varianceA > varianceB) {
				return small;
			}
			else {
				return !small;
			}
		}
	}
}

/*check if finished*/
bool ifFinished(vector<InvokedTask> tasks){
	for(auto ti: tasks){
		if(tasks.checkState() != finished)
			return false;
		}
		return true;
}

int main(int argc, char const *argv[])
{
	int cycle = 0;
	Clock* clock = Clock::getClock();
	double Freq = 1;
	// Core cores[64] ;
	// for (int i = 0; i < 64; ++i)
	// {
	// 	Core ctemp(Freq, i);
	// 	core[i] = ctemp;
	// }

	vector<InvokedTask> tasks;
	vector<vector<double>> taskGraph;
	int taskNum;
	
	Application testApp("/app/wen/7.txt");
	vector<Task> row = testApp.getTasks();
	for (vector<Task>::iterator iter = row.begin(); iter != row.end(); ++iter)
	{
		tasks.push_back(InvokedTask(*iter));
	}
	
	taskGraph = testApp.getTaskGraph();
	taskNum = tasks.size();
	int app_id = row.getId();
	
	Map map(1, 8, 8);
	squareMapping(map, tasks);

	count<<"start simple simulation"<<endl;
	while(1){
		cycle++;
		for (int i = 0; i < taskNum; i++) {/*row*/
			InvokedTask* t_cur = &tasks[i];
			Core* c = map->getSpecificCore(t_cur->getLandlord());
			int exe_speed = c->getFreq();
			vector<double> send;
			vector<double> recieve;
			vector<double> need = t_cur->unprocessedData();
			for (int j = 0; j < taskNum; ++j)/*colunm*/
			{
				if (i!=j && need[j]!=0)
				{
					int distance = tasks[j].getLandlord().Manhattan(t_cur->getLandlord());
					send.push_back(1/distance);
					recieve.push_back(1/distance);
				}
				else{
					send.push_back(0);
					recieve.push_back(0);
				}
			}
			bool tick = true;
			vector<int> parent = t_cur->getParents();
			State s = t_cur->checkState();
			switch (s) {
			case waiting:
				if (parent.empty()) {
					t->setState(executing);
				}
				else {
					tick = false;
					for (auto pi : parent) {
						if (tasks[pi].checkState() == tranfering) {
							tick = true;
						}
					}
					if (tick) {
						t->setState(receiving);
					}
				}
				break;
			case receiving:
				t_cur->nRound(0, recieve);
				c->turnOn_router();
				break;
			case executing:
				t_cur->nRound(exe_speed, vector<double>(taskNum,0));
				if(t_cur-> getExecutionVolumeFinished() == t_cur->getExecutionVolume())
					c->turnOff();
				break;
			case tranfering:
				t_cur->nRound(0, send);
				c->turnOn_router();
				break;
			case finished:
				t_cur->nRound(0, vector<double>(taskNum, 0));
				c->turnOff_router();
				break;
			}
		}
		/*update Task Graph*/
		for (int i = 0; i < taskNum; i++) {
			taskGraph[i] = tasks[i].unprocessedData();
		}
		/*print task Graph*/
		for (auto i : taskGraph) {
			for (auto j : i) {
				cout << j << '\t';
			}
			cout << endl;
		}

		clock->tick();
		
		if(ifFinished(tasks)){
			count<<"total_cycle:"<< cycle<<endl;
			count<<"running clock"<< clock->now() << endl;
			break;
		}


	}/*while end*/


	return 0;
}