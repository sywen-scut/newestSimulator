#pragma once
#include "Map.h"
#include "Clock.h"
#include "Region.h"
using namespace std;

class Event {
private:
	vector<InvokedTask> tasks;
	vector<vector<double>> taskGraph;
	int arrivalTime;
	int taskNum;
	int id;
	
	State state;
	int startTime;
	int finishTime;
	int migratetimes = 0;
	int migrate_distance = 0;
	
public:
	double CCR;
	Region region;
	double inst_cur;
	double trans_cur;
	Event(Application app);
	bool arrive(int now);
	void updateTaskGraph();
	void showTaskGraph();
	/*
	if the map is clean, the start running action fails
	if the map has been changed, but can't pass the thermal check, 
	the action need to abort
	otherwise, it starts successfully
	*/
	bool startRunning(Map* map,int type, Vertex point, int regsize);
	
	// Vertex book(Map map, int bubbleNum = 0,vector<Vertex> x = vector<Vertex>(0));// find the left top corner vertex
	bool mapping(Map* map,int type, Vertex point,int regsize);
	/*
	the logic here is that,
	true means the map has changed
	false means the map is untouched(clean)
	*/
	void finish(Map* map);
	bool ifFinished(vector<InvokedTask> tasks);
	void unhookFromMap(Map* map);
	void nRound(Map* map);
	bool hasFinished();
	void wait();
	int getArrivalTime();
	int getStartTime();
	int getFinishTime();
	int getNumTasks();
	
	vector<InvokedTask> getTasks();

	int getId();
	void swapTask(Map* map, InvokedTask* t1, InvokedTask* t2);
	bool setRegion(Map* map, Vertex point, int region_width, int region_length);
	void releaseRegion(Map* map);
	/*Mapping method*/
	bool CASqA(Map* map);//state of art mapping method
	bool squareMapping(Map* map, Vertex point);
	bool chessMapping(Map* map, Vertex point);
	bool nonContigousMapping(Map* map);
	
	// bool withBubbleMapping(Map* map, Vertex point, int regsize);
	bool centralBubbleMapping(Map* map, Vertex point, int regsize);
	bool egdeBubbleMapping(Map* map,Vertex point, int regsize);
	
	bool initialMapping(Map* map, Vertex point);
	/*Migration method*/
	bool squareMigration(Map* map);
	bool chessMigration(Map* map);
	bool globalColdest(Map* map, vector<int> hot_tid);
	bool localColdest(Map* map, vector<int> hot_tid);
	bool localNeighbor(Map* map, vector<int> hot_tid);
	bool dvfs(Map* map, vector<int> hot_tid);
	bool neighborCol(Map* map, vector<int> hot_tid);

	
};
