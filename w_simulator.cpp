#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>  
#include <stdio.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <unistd.h>
#include "Clock.h"
#include "Map.h"

using namespace std;
int migrate_distance = 0;

/*Migration Pattern*/
vector<InvokedTask> squareMapping(Map* map, vector<InvokedTask> tasks){
	int count = 0;
	int size = tasks.size();
	int size_t = floor(sqrt(size));
	cout<<"Square: "<<size_t<<"*"<<size_t<<endl;
		for (int j =0; j < map->getLength(); j++) {
			for (int k =0; k < size_t; k++) {
				Vertex temp(j, k);
				if (!map->getSpecificCore(temp)->active) {
					map->turnOnCore(tasks[count], temp);
					tasks.at(count).startRunning(temp);
					count++;
       
					//map one more task 	
				}if(count == tasks.size())
				   break;
			}if(count == tasks.size())
			   break;
		}
		

	if (count == tasks.size()) {
		return tasks;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
				cout<<" map failed" <<endl;
				exit(1);
		}
	}
} 

vector<InvokedTask> squareMigration(Map* map, vector<InvokedTask> tasks){
	
	
	int current_location = tasks[0].getLandlord().l;
	/*turn off active core*/
	for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) 
		map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
				
	}
	/*Remapping*/
	int count = 0;
	int size = tasks.size();
	int size_t = floor(sqrt(size));
	cout<<"do Square Migration: "<<size_t<<"*"<<size_t<<endl;
	int j=0;
	if (current_location == 0)
		/*move up*/
		j = size_t ; 
	else
		j = 0 ;
		/*move down*/
	for (; j < map->getLength(); j++) {
			for (int k =0; k < size_t; k++) {
				Vertex temp(j, k);
				if (!map->getSpecificCore(temp)->active) {
					map->turnOnCore(tasks[count], temp);
					tasks.at(count).continueRunning(temp);
					count++;
       
					//map one more task 	
				}if(count == tasks.size())
				   break;
			}
			if ( count == tasks.size())
			   break;
		}
		

	if (count == tasks.size()) {
		return tasks;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
				cout<<" map failed" <<endl;
				exit(1);
		}
	}
}	

vector<InvokedTask> chessMapping(Map* map,vector<InvokedTask> tasks){
	int count = 0;
	int size = tasks.size();
	int size_t = 2*ceil (sqrt(size));
	cout<<"Chessmap: "<<size_t<<"*"<<size_t<<endl;
		for (int j =0; j < map->getLength(); j+=1) {
			if (j%2 == 0){
				for (int k =0; k < size_t; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
						//map one more task 	
					}if(count == tasks.size())
							break;
				}
			}
			else{
				for (int k =1; k < size_t; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).startRunning(temp);
						count++;
						//map one more task 	
					}if(count == tasks.size())
							break;
				}
			}
			if(count == tasks.size())
				break;

		}
	if (count == tasks.size()) {
		return tasks;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
			cout<<" map failed" <<endl;
			exit(1);
		}
	}

}


vector<InvokedTask> chessMigration(Map* map,vector<InvokedTask> tasks){
	int current_location = tasks[0].getLandlord().w;

	for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
	}	

	int count = 0;
	int size = tasks.size();
	int size_t = 2*ceil (sqrt(size));
	int jplus = 0;
	if (current_location == 0)
	{
		jplus = 1;
		cout<<"Right shift for first line"<<endl;
	}
	else
	{
		jplus = 0;
		cout<<"Left shift for first line"<<endl;
	}
	cout<<"do chessMigration: "<<endl;
		for (int j =0; j < map->getLength(); j+=1) {

			if ((j +jplus) %2 == 0){
				for (int k =0; k < size_t; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).continueRunning(temp);
						count++;
						//map one more task 	
					}if(count == tasks.size())
							break;
				}
			}
			else{
				for (int k =1; k < size_t; k+=2) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active) {
						map->turnOnCore(tasks[count], temp);
						tasks.at(count).continueRunning(temp);
						count++;
						//map one more task 	
					}if(count == tasks.size())
							break;
				}
			}
			if(count == tasks.size())
				break;

		}
	if (count == tasks.size()) {
		return tasks;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release();
			}
			cout<<" map failed" <<endl;
			exit(1);
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
/*check if temperture exceed threshold*/
bool CheckTemperture(Map* map){
	double threshold = 80;
	for(int k = 0; k < map->getLength(); k++) {
		for (int j = 0; j < map-> getWidth(); j++) {
			Vertex temp(k, j);
			if (map->getSpecificCore(temp)->getTaskId() != -1)
			{
				if(map->getSpecificCore(temp)->temperture > threshold)
					return true;
				}
			}
		}
		return false;
}
/*check which task exceed threshold*/
vector<int> CheckHotSpots(Map* map){
	vector<int> hot_tid;
	Core* core_temp;
	double threshold = 80;
	for(int k = 0; k < map->getLength(); k++) {
		for (int j = 0; j < map-> getWidth(); j++) {
				Vertex temp(k, j);
				core_temp = map->getSpecificCore(temp);
				if (core_temp->active)
				{
					if(core_temp->temperture > threshold)
						hot_tid.push_back(core_temp->getTaskId());
				}
		}
	}	
		return hot_tid;
}

vector<InvokedTask> GlobalColdest(Map* map, vector<InvokedTask> tasks){
	
	vector<int> hot_tid  = CheckHotSpots(map);
	/*Find coolest core and migrate*/
	cout<<"do GlobalColdest Migration: "<<endl;
	double coldest_tmp = 80;
	Vertex  previous_vertex;
	Vertex coldest_vertex;
	for (auto tid : hot_tid) {
		cout <<"Hot task:"<<tid<<endl;
		previous_vertex = tasks[tid].getLandlord();
		map->turnOffCore(tasks[tid].getLandlord());
		tasks[tid].release();
		for(int k = 0; k < map->getLength(); k++) {
			for (int j = 0; j < map-> getWidth(); j++) {
				Vertex temp(k, j);
				if(map->getSpecificCore(temp)->getTaskId() == -1){
					double tmp_cur = map->getSpecificCore(temp)->temperture;
					if ( tmp_cur < coldest_tmp)
					{
						coldest_tmp = tmp_cur;
						coldest_vertex = temp;
					}
				}
			}
		}
		/*add cost to total migrate distance*/
		migrate_distance += coldest_vertex.Manhattan(previous_vertex);
		map->turnOnCore(tasks[tid], coldest_vertex);
		tasks[tid].continueRunning(coldest_vertex);
		coldest_tmp = 80;/*Find next coldest core*/
	}			
	return tasks;
}	

/*check if finished*/
bool ifFinished(vector<InvokedTask> tasks){
	for (int i = 0; i < tasks.size();i++) {
		if (tasks[i].checkState() != finished)
			return false;
		}
		return true;
}


int main(int argc, char const *argv[])
{
	int cycle = 0;
	int migratetimes = 0;
	Clock* clock = Clock::getClock();
	double Freq = 1;
	vector<InvokedTask> tasks;
	vector<vector<double>> taskGraph;
	// Core cores[64] ;
	// for (int i = 0; i < 64; ++i)
	// {
	// 	Core ctemp(Freq, i);
	// 	core[i] = ctemp;
	// }
	int taskNum;

	cout<<"start simple simulation"<<endl;

	string appName = argv[1];
	string app_filepath = "/home/chris/simulator/app/wen/" + appName+".txt";
	Application testApp(app_filepath);
	cout<<"Input: "<< app_filepath<< endl;
	vector<Task> row = testApp.getTasks();
	for (vector<Task>::iterator iter = row.begin(); iter != row.end(); ++iter)
	{
		tasks.push_back(InvokedTask(*iter));
	}
	
	taskGraph = testApp.getTaskGraph();
	taskNum = tasks.size();
	/*Running frequency 1, mesh size 8*8*/
	Map map(1, 8, 8);
	//tasks = squareMapping(&map, tasks);
	tasks = squareMapping(&map, tasks);
             map.showMap();
	Map* map_t = &map;
	
	string Ptrace_filepath = "/home/chris/simulator/Ptrace/"+appName+".ptrace";
	
	FILE* fp = fopen(Ptrace_filepath.c_str(),"w");
	if( fp==NULL )
		{cout<<"can't find output path"<<endl;
		exit(0);
	}
	else
		cout << "Output: " <<Ptrace_filepath << endl;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			fprintf(fp, "P_%d_%d \t", j, i);
		}

	}
	fprintf(fp, "\n");
	fclose(fp);
	string cmd_string = "/home/chris/HotSpot-6.0/simulator.sh "+appName;
	char hotspot_cmd[100];
	sprintf(hotspot_cmd,"%s", cmd_string.c_str());

	string ttrace_path = "/home/chris/HotSpot-6.0/"+appName+".ttrace";
	
	cout<<"start running"<<endl;
	
	while(1){
		cycle++;


		for (int i = 0; i < taskNum; i++) {/*for each task*/
			InvokedTask* t_cur = &tasks[i];
			Vertex v_cur = t_cur->getLandlord();
		       	Core* c = map_t->getSpecificCore(v_cur);
			int exe_speed = c->getFreq();
			vector<double> send;
			vector<double> recieve;
			vector<double> need = t_cur->unprocessedData();
			for (int j = 0; j < taskNum; ++j)/*colunm*/
			{
				if (need[j]!=0)
				{
					int distance = tasks[j].getLandlord().Manhattan(t_cur->getLandlord());
					if (need[j]>0) 
					{
						
						recieve.push_back(1.0/distance);
						send.push_back(0);
						
						
					}
					if(need[j]<0)
					{
						recieve.push_back(0);
						send.push_back(1.0/distance);
					}
					
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
					t_cur->setState(executing);
				}
				else {
					tick = false;
					for (auto pi : parent) {
						if (tasks[pi].checkState() == tranfering) {
							tick = true;
						}
					}
					if (tick) {
						t_cur->setState(receiving);
					}
				}
				break;
			case receiving:
				c->turnOn_router();
				for (int i = 0; i < taskNum; ++i)
				{
					if (tasks[i].checkState() == tranfering || tasks[i].checkState() == finished)
					{
						t_cur-> dataProcessed[i] += recieve[i];
					}
				}
					t_cur->nRound(0, taskNum);
				
				break;
			case executing:
				c->turnOff_router();
				c->turnOn_Processor();
				t_cur ->executionVolumeFinished += exe_speed;
				t_cur->nRound(exe_speed, taskNum);
				break;
			case tranfering:
				c->turnOff_Processor();
				c->turnOn_router();
				for (int i = 0; i < taskNum; ++i)
				{
					t_cur-> dataProcessed[i] -= send[i];
				}
				t_cur->nRound(0,taskNum);
				break;
			case finished:
				t_cur->nRound(0, taskNum);
				c->turnOff_router();
				break;
			}
			
			/*cout<<"Tid: "<<t_cur -> getId() <<"  finished inst: "<<t_cur->getExecutionVolumeFinished()<<endl;
			cout<<"dataProcessed: ";
			for (auto di: t_cur-> dataProcessed)
			{
				cout<<di<<' ';
			}
			cout<<endl;*/
			

		}
		/*cout<<"cycle:"<< cycle<<endl;
		cout<<"Tid: "<<3<<"  finished inst: "<<tasks[3].getExecutionVolumeFinished()<<endl;
			cout<<"dataProcessed: ";
			for (auto di: tasks[3]. dataProcessed)
			{
				cout<<di<<' ';
			}
			cout<<endl;
		cout<<"Tid: "<<9<<"  finished inst: "<<tasks[9].getExecutionVolumeFinished()<<endl;
			cout<<"dataProcessed: ";
			for (auto di: tasks[9].dataProcessed)
			{
				cout<<di<<' ';
			}
			cout<<endl;
		cout<<endl;*/
		/*update Task Graph*/
		for (int i = 0; i < taskNum; i++) {
			taskGraph[i] = tasks[i].unprocessedData();
		}
		
		/*write power trace*/
		fp = fopen(Ptrace_filepath.c_str(),"a+");
		
		map_t->writePtrace(fp);
		fclose(fp);
		if (cycle % 50 == 0 && cycle !=0)
		{
			system(hotspot_cmd);
			map_t -> getTemperature(ttrace_path);
			
			if(CheckTemperture(map_t)){
				cout<<"cycle : " << cycle<<endl;
				// tasks = squareMigration(map_t, tasks);
				tasks = GlobalColdest(map_t, tasks);
				migratetimes++;
				map_t->showMap();
				cout<<"Overhead (hopcounts): "<<migrate_distance<<endl;
			}
		}
		
		clock->tick();

		if(ifFinished(tasks)){
			cout<<"total_cycle:"<< cycle<<endl;
			cout<<"running clock:"<< clock->now() << endl;
			cout<<"Migratetimes: "<<migratetimes<<endl;
			cout<<"Overhead (hopcounts): "<<migrate_distance<<endl;
			break;
			}


	}/*while end*/
			map_t->showMap();

	return 0;
}
