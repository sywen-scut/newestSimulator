#include "Event.h"
#include <numeric>
#include <time.h>
using namespace std;

extern double threshold;

Event::Event(Application app) {//warning: havn't initialize shape yet
	vector<Task> row = app.getTasks();
	for (vector<Task>::iterator iter = row.begin(); iter != row.end(); ++iter)
		{  
		tasks.push_back(InvokedTask(*iter));
		}
	taskGraph = app.getTaskGraph();
	arrivalTime = app.getArrivalTime();
	taskNum = tasks.size();
	id = app.getId();
	CCR = app.getCCR();
}

bool Event::arrive(int now) {
	if (arrivalTime == now) {
		return true;
	}
	return false;
}

void Event::updateTaskGraph(){
	
	for (int i = 0; i < taskNum; i++) {
		taskGraph[i] = tasks[i].unprocessedData();
	}
	
}

void Event::showTaskGraph()
{
	for (auto i : taskGraph) {
		for (auto j : i) {
			cout << j << '\t';
		}
		cout << endl;
	}
}

bool Event::startRunning(Map* map,int type, Vertex point, int regsize) {
	state = running;
	startTime = Clock::getClock()->now();
	if (mapping(map,type, point, regsize)) {
		return true;	
	}
	// unhookFromMap(map);
	return false;
}
	

bool Event::mapping(Map* map, int type, Vertex point, int regsize) {
	/*default : square contigous map*/
	// Vertex pin = book(*map);
	switch (type) {
	
	case 1:
		return squareMapping(map,point);
	case 2:
		return chessMapping(map,point);
	case 3:
		return nonContigousMapping(map);
	case 4:
		return egdeBubbleMapping(map,point,regsize);
	case 5:
		return centralBubbleMapping(map,point,regsize);	
	case 6:
		return nonContigousMapping(map);
	}
}

void Event::finish(Map* map) {
	state = finished;
	finishTime = Clock::getClock()->now();
	unhookFromMap(map);
}

bool Event::ifFinished(vector<InvokedTask> tasks){
	for (int i = 0; i < tasks.size();i++) {
		if (tasks[i].checkState() != finished)
			return false;
		}
		return true;
}

void Event::unhookFromMap(Map* map) {
	for (int i = 0; i < taskNum;i++) {
		if (tasks[i].checkState() != unmapped) {
			map->turnOffCore(tasks[i].getLandlord());
			tasks[i].release();
		}
	}
	releaseRegion(map);
	
}


void Event::nRound(Map* map) {
	this->inst_cur = 0;
	this->trans_cur = 0;
	for (int i = 0; i < taskNum; i++) {/*for each task*/
			InvokedTask* t_cur = &tasks[i];
			Vertex v_cur = t_cur->getLandlord();
		       	Core* c = map->getSpecificCore(v_cur);
			double exe_speed = c->getFreq() * t_cur->getIpc();
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
				// c->turnOn_Processor();
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
				// c->turnOn_Processor();
				c->turnOn_router();
				for (int i = 0; i < taskNum; ++i)
				{
					if (tasks[i].checkState() == tranfering || tasks[i].checkState() == finished)
					{
						t_cur-> dataProcessed[i] += recieve[i];
						// add to current trans vol
						this->trans_cur += recieve[i];
					}
				}
					t_cur->nRound(0, taskNum);
				
				break;
			case executing:
				c->turnOff_router();
				c->turnOn_Processor();
				t_cur ->executionVolumeFinished += exe_speed;
				this->inst_cur += exe_speed;
				t_cur->nRound(exe_speed, taskNum);
				break;
			case tranfering:
				// c->turnOn_Processor();
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
				c->turnOff_Processor();
				break;
			}	
	}
	updateTaskGraph();
	
	if (ifFinished(tasks)) {
		finish(map);
		
		cout<<"APP "<<getId()<<" finished"<<endl;
		cout<<" response time "<<finishTime - arrivalTime<<endl;
		cout<<" waiting time: " << startTime - arrivalTime<<endl;
		cout<<" execution time: " << finishTime - startTime<<endl;
		cout<<" migratetimes: "<<migratetimes<<endl;
		cout<<" migrate_distance: "<<migrate_distance<<endl;

		// FILE* rfp = fopen("/home/chris/simulator/Result","a+");
		// fprintf(rfp, "app%d\t", getId());
		// fprintf(rfp, "response %d\t", finishTime - arrivalTime);
		// fprintf(rfp, "wait %d\t", startTime - arrivalTime);
		// fprintf(rfp, "exe %d\t", finishTime - startTime);
		// fprintf(rfp, "migratetime %d\n", migratetimes);
		// fclose(rfp);
	}
}

bool Event::hasFinished() {
	return state == State::finished;
	/*finished or not*/
}

void Event::wait() {
	state = waiting;
}

int Event::getArrivalTime() {
	return arrivalTime;
}

int Event::getStartTime() {
	return startTime;
}

int Event::getFinishTime() {
	return finishTime;
}

int Event::getNumTasks() {
	return  taskNum;
}

vector<InvokedTask> Event::getTasks() {
	return tasks;
}

int Event::getId() {
	return id;
}

void Event::swapTask(Map* map, InvokedTask* t1, InvokedTask* t2){
		Vertex v1 = t1->getLandlord();
		Vertex v2 = t2->getLandlord();
		map->turnOffCore(v1);
		t1->release2();
		map->turnOffCore(v2);
		t2->release2();
		map->turnOnCore(*t1, v2);
		t1->continueRunning(v2);
		map->turnOnCore(*t2,v1);
		t2->continueRunning(v1);
}

bool Event::setRegion(Map* map, Vertex point, int region_width, int region_length){

	
	if (point.w+region_width <= map->getWidth())
		if (point.l + region_length <= map->getLength())
		{
			Region r_tmp(point, region_width, region_length);
			for (int j = r_tmp.start_p.l; j <= r_tmp.end_p.l; j++) {
				for (int k = r_tmp.start_p.w; k <= r_tmp.end_p.w; k++) {
					Vertex temp(j, k);
					if(map->getSpecificCore(temp)->occupied)
						return false;
				}
			}
			this->region = r_tmp;
			for (int j = this->region.start_p.l; j <= this->region.end_p.l; j++) {
				for (int k = this->region.start_p.w; k <= this->region.end_p.w; k++) {
					Vertex temp(j, k);
					map->getSpecificCore(temp)->occupy();
				}
			}
			return true;
		}
	return false;
}

void Event::releaseRegion(Map* map){
	if (region.w > 0 && region.l > 0)
	{
		for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			for (int k = region.start_p.w; k <= region.end_p.w; k++) {
					Vertex temp(j, k);
					map->getSpecificCore(temp)->unoccupy();
			}
		}
	}
}

int step2middle(int length){
	if (length%2 == 1) 
		return length>>1;
	else 
		return (length>>1)-1;
}

bool Event::CASqA(Map* map){
	//int Rmax = floor(ceil(sqrt(tasks.size()))/2);
	int Rmax = floor(sqrt(tasks.size()))+2;
	Vertex FN (region.start_p.l+step2middle(region.l), region.start_p.w+step2middle(region.w));
	vector<InvokedTask> MAP;
	queue<InvokedTask> MET;
	vector<InvokedTask> UNM;
	int tf_id;
	double heaviest = 0;
	for(auto t:tasks)
	{
		UNM.push_back(t);
		double temp_weight = 0;
		/**Find the task with heaciest communication vol.**/
		
		// for(auto edge:t.getWithNeibors())
		// {
		// 	temp_weight += fabs(edge);
		// }
		// if (temp_weight>heaviest) {
		// 	heaviest = temp_weight;
		// 	tf_id = t.getId();
		// }	
		
	}
	tf_id = 0;
	if (!map->getSpecificCore(FN)->active) 
	{
		map->turnOnCore(tasks[tf_id], FN);
		tasks[tf_id].startRunning(FN);
		MAP.push_back(tasks[tf_id]);
		for (vector<InvokedTask>::iterator iter = UNM.begin(); iter != UNM.end();) 
		{
			if ((*iter).getId() == tf_id) {
				iter = UNM.erase(iter);
				break;
			}
			else {
				iter++;
			}
		}
	}
	else
		return false;

	int MD=1;
	int r=1;
	
	vector<int> neibour = tasks[tf_id].neibours;
	for(auto p:neibour)
	{
		for (vector<InvokedTask>::iterator iter = UNM.begin(); iter != UNM.end();) 
			{
					if ((*iter).getId() == p) {
						MET.push(*iter);
						iter = UNM.erase(iter);
						break;
						
					}
					else {
						iter++;
					}
			}	
				
	}
	
	// cout<<"METsize"<<MET.size()<<endl;
	for(;r<=Rmax;r++){
		for(MD =1; MD<=4*r;){
			
			InvokedTask tc = MET.front();
			InvokedTask tp = tasks[tf_id];
			for(auto tparent : MAP)
			{
				if(tc.getWithNeibors().at(tparent.getId())!=0)
					tp = tparent;
			}
			
			//select one node for single tc
			int k = ((FN.w-r)<region.start_p.w)?region.start_p.w:(FN.w-r);
			for (; k <= FN.w+r && k<=region.end_p.w; k++ ) {
				
				int j = ((FN.l-r)<region.start_p.l)?region.start_p.l:(FN.l-r);
				for (; j <= FN.l+r&& j<=region.end_p.l; j++) {
					Vertex temp(j, k);
					if (!map->getSpecificCore(temp)->active && temp.Manhattan(tasks[tp.getId()].getLandlord())==MD)
					{
						map->turnOnCore(tasks[tc.getId()], temp);
						tasks.at(tc.getId()).startRunning(temp);
						MET.pop();
						MAP.push_back(tc);
						
						if (tc.neibours.size()-1!= 0) {
						for(auto p:tc.neibours)
						{
							for (vector<InvokedTask>::iterator iter = UNM.begin(); iter != UNM.end();) 
								{
									if ((*iter).getId() == p) {
										MET.push(*iter);
										iter = UNM.erase(iter);
										MD = 1;
										break;
										
									}
									else {
										iter++;
									}
								}	
								
						}
						}
						break;
						//we could further add ICEB evaluation here
					}
				}if(tasks[tc.getId()].checkState() != unmapped)
						break;
			}
			if(tasks[tc.getId()].checkState() == unmapped)
				MD++;
			if(MAP.size() == tasks.size())
				break;
		}
		if(MAP.size() == tasks.size())
			break;	
	}
	if (MAP.size() != tasks.size()) {
		/*unhook map*/
		unhookFromMap(map);
		cout<<" map failed" <<endl;
		return false;
	}
	else
		return true;	
}

bool Event::squareMapping(Map* map, Vertex point){
	int size = tasks.size();
	int region_length = floor(sqrt(size));
	int region_width = 2*ceil(sqrt(size));
	if(region_length*region_width<2*size)
		region_length+=1;
	int count = 0;
	if (!setRegion(map, point, region_width, region_length))
	{
		return false;
	}
	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"squaremap: "<<region.l<<"*"<<region.l<<endl;	
	if(CASqA(map)){
		Vertex temp;
		for (int i = 0; i < tasks.size();i++) {
			temp = tasks[i].getLandlord();
			Core* c = map->getSpecificCore(temp);
			c->setFreq(1+(double)(region.area - taskNum)/(taskNum*1.5));
		}
		return true;
	}
	else
		return false;
} 

bool Event::chessMapping(Map* map, Vertex point){
	int size = tasks.size();
	int region_width = 2*ceil(sqrt(size));
	int region_length = floor(sqrt(size));
	int count = 0;
	if (!setRegion(map, point, region_width, region_length))
	{
		return false;
	}
	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.start_p)->active||map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"Chessmap: "<<region.w<<"*"<<region.l<<endl;
		for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			if (j%2 == 0){
				for (int k = region.start_p.w; k <= region.end_p.w; k+=2) {
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
				for (int k = region.start_p.w + 1; k <= region.end_p.w; k+=2) {
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
		//Adjust for star shape application
		int mtid = floor(size/2);
		//swapTask(map, 0,mtid);

		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" map failed" <<endl;
		return false;
		}
}
bool Event::nonContigousMapping(Map* map){
	int count = 0;
		for (int j =0; j < map->getLength(); j++) {
			for (int k =0; k < map->getWidth(); k++) {
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
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		// cout<<" map failed" <<endl;
		return false;
	}
}
bool Event::squareMigration(Map* map){
	int current_location = tasks[0].getLandlord().w;
	/*Remapping*/
	cout<<"do Square Migration for app"<<getId()<<endl;
	int j=ceil(sqrt(tasks.size()));
	Vertex  cur_v;
	if (current_location < j)
		/*move to right*/
		for (int i = 0; i < tasks.size();i++) {
			if (tasks[i].checkState() != unmapped)	
			{	cur_v = tasks[i].getLandlord();
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release2();
				Vertex temp(cur_v.l, cur_v.w+j);
				map->turnOnCore(tasks[i], temp);
				tasks.at(i).continueRunning(temp);
			}
		}
	else/*move to left*/
		for (int i = 0; i < tasks.size();i++) {
			if (tasks[i].checkState() != unmapped)
			{
				cur_v = tasks[i].getLandlord();
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release2();
				Vertex temp(cur_v.l, cur_v.w-j);
				Core* c = map->getSpecificCore(temp);
				map->turnOnCore(tasks[i], temp);
				tasks.at(i).continueRunning(temp);
				c->setFreq(1+(double)(region.area - taskNum)/(taskNum*1.5));
			}
		}
	migratetimes++;
	return true;
	
}

bool Event::chessMigration(Map* map){
	int current_location = tasks[1].getLandlord().w;
	/*turn off active cores*/
	for (int i = 0; i < tasks.size();i++) {
				if (tasks[i].checkState() != unmapped) {
				map->turnOffCore(tasks[i].getLandlord());
				tasks[i].release2();
			}
	}	

	int count = 0;
	int jplus = 0;
	if (current_location == 2)
	{
		jplus = 1;
		// cout<<"Right shift for first line"<<endl;
	}
	else
	{
		jplus = 0;
		// cout<<"Left shift for first line"<<endl;
	}
	cout<<"do chessMigration for app"<<getId()<<endl;
	for (int j = region.start_p.l; j <= region.end_p.l; j++) {
			if ((j +jplus) %2 == 0){
				for (int k = region.start_p.w; k <= region.end_p.w; k+=2) {
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
				for (int k = region.start_p.w + 1; k <= region.end_p.w; k+=2) {
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
		migratetimes++;
		int mtid = floor(tasks.size()/2);
		// swapTask(map, 0 ,mtid);
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" migration failed" <<endl;
		return false;
		}
}

bool Event::globalColdest(Map* map, vector<int> hot_tid){
	/*Find coolest core and migrate*/
	cout<<"do GlobalColdest Migration for app"<<getId()<<endl;
	double coldest_tmp = threshold;
	Vertex  previous_vertex;
	Vertex coldest_vertex;
	for (auto tid : hot_tid) {
		// cout <<"Hot task:"<<tid<<endl;
		previous_vertex = tasks[tid].getLandlord();
		map->turnOffCore(tasks[tid].getLandlord());
		tasks[tid].release2();
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
		if (coldest_vertex.w == -1)
		{
			/*decrease the frequency, nowhere to migrate*/
			map->turnOnCore(tasks[tid], previous_vertex);
			map->getSpecificCore(previous_vertex)->setFreq(0.5);
			tasks[tid].continueRunning(previous_vertex);
			// cout<<"decrease the frequency, nowhere to migrate"<<endl;
		}
		else{
			/*add cost to total migrate distance*/
			migrate_distance += coldest_vertex.Manhattan(previous_vertex);
			map->turnOnCore(tasks[tid], coldest_vertex);
			map->getSpecificCore(coldest_vertex)->setFreq(1);
			tasks[tid].continueRunning(coldest_vertex);
			// cout<<"migrate to dark core"<<endl;
			coldest_tmp = threshold;/*Find next coldest core*/
			coldest_vertex.w = -1;
		}
		
	}	
	migratetimes++;		
	return true;
}

bool Event::neighborCol(Map* map, vector<int> hot_tid){
	/*Find adjacent coolest core and migrate*/
	cout<<"do neighborCol Migration for app"<<getId()<<endl;
	double coldest_tmp = threshold;
	Vertex  previous_vertex;
	Vertex coldest_vertex;
	for (auto tid : hot_tid) {
		// cout <<"Hot task:"<<tid<<endl;
		previous_vertex = tasks[tid].getLandlord();
		map->turnOffCore(tasks[tid].getLandlord());
		tasks[tid].release2();
		for(int k = previous_vertex.l+1; k >= previous_vertex.l-1; k--) {
			for (int j = previous_vertex.w+1; j >= previous_vertex.w-1; j--) {
				if(k >= 0 && k <map->getLength() && j >=0 && j<map->getWidth())
				{	
					Vertex temp(k, j);
					// || map->getSpecificCore(temp)->getApplicationId() == id
					if(!map->getSpecificCore(temp)->occupied && (map->getSpecificCore(temp)->getTaskId() == -1 )) 
					{
						double tmp_cur = map->getSpecificCore(temp)->temperture;
						if ( tmp_cur < coldest_tmp)
						{
							coldest_tmp = tmp_cur;
							coldest_vertex = temp;
						}
					}
				}
			}
		}
		if (coldest_vertex.w == -1)
		{
			/*decrease the frequency, nowhere to migrate*/
			map->turnOnCore(tasks[tid], previous_vertex);
			map->getSpecificCore(previous_vertex)->setFreq(0.5);
			tasks[tid].continueRunning(previous_vertex);
			// cout<<"decrease the frequency, nowhere to migrate"<<endl;
		}
		else{
			/*add cost to total migrate distance*/
			migrate_distance += coldest_vertex.Manhattan(previous_vertex);
			if(map->getSpecificCore(coldest_vertex)->getTaskId() == -1){
				map->turnOnCore(tasks[tid], coldest_vertex);
				map->getSpecificCore(coldest_vertex)->setFreq(1);
				map->getSpecificCore(coldest_vertex)->occupy();
				tasks[tid].continueRunning(coldest_vertex);
			}
			
			else
			{
				tasks[tid].continueRunning(previous_vertex);
				InvokedTask* t1 = &tasks[map->getSpecificCore(coldest_vertex)->getTaskId()] ;
				InvokedTask* t2 = &tasks[tid];
				swapTask(map , t1, t2);
			}
			
			
			// cout<<"migrate to dark core"<<endl;
			coldest_tmp = threshold ;/*Find next coldest core*/
			coldest_vertex.w = -1;
		}
		
	}
		
	migratetimes++;		
	return true;
}
bool Event::initialMapping(Map* map, Vertex point){
	int size = tasks.size();
	int region_width = 4;
	int region_length =4;  
	
	if (!setRegion(map, point, region_width, region_length))
	{
		return false;
	}

	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"Region: "<<region.w<<"*"<<region.l<<endl;	

	if(CASqA(map)){
		Vertex temp;
		for (int i = 0; i < tasks.size();i++) {
			temp = tasks[i].getLandlord();
			Core* c = map->getSpecificCore(temp);
			c->setFreq(0.7);
		}
		return true;
	}
	else
		return false;
}
bool Event::egdeBubbleMapping(Map* map, Vertex point, int regsize){
	 
	int size = tasks.size();
	
	
	int region_width = 14 ;
	int region_length = 7;        

	
	// if (point.l == 6 && point.w == 0) {
	// region_width = 6 ;
	// region_length = 2; 
	// }

	int count = 0;
	if (!setRegion(map, point, region_width, region_length))
	{
		return false;
	}

	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"Region: "<<region.w<<"*"<<region.l<<endl;	

	if(CASqA(map)){
		Vertex temp;
		for (int i = 0; i < tasks.size();i++) {
			temp = tasks[i].getLandlord();
			Core* c = map->getSpecificCore(temp);
			c->setFreq(1+(double)(region.area - taskNum)/(taskNum*1.5));
		}
		return true;
	}
	else
		return false;
} 

bool Event::centralBubbleMapping(Map* map, Vertex point, int regsize){
	int size = tasks.size();
	int region_width ;
	int region_length; 	
	region_width = 10;
	region_length = 10;
	if (!setRegion(map, point, region_width, region_length))
	{
		return false;
	}

	Vertex top_right_p(region.start_p.l,region.end_p.w);
	if (map->getSpecificCore(region.end_p)->active||map->getSpecificCore(top_right_p)->active)
	{
		return false;
	}
	cout<<"Region: "<<region.w<<"*"<<region.l<<endl;
	int bubbles = region.area - size;
	Vertex centernode(region.start_p.l+step2middle(region.l), region.start_p.w+step2middle(region.w));
	
		for (int j = centernode.l; j <= centernode.l +1; j++) {
			for (int k = centernode.w; k <= centernode.w +1; k++) {
				Vertex temp(j, k);
				map->getSpecificCore(temp)-> setbubble();
				bubbles-=1;
				if(bubbles == 0) break;
			}if(bubbles == 0) break;
		}
	//Map tasks following id order		
	int count = 0;
	for (int j = region.start_p.l; j <= region.end_p.l; j++) {
		for (int k = region.start_p.w; k <= region.end_p.w; k++) {
			Vertex temp(j, k);
			Core* c = map->getSpecificCore(temp);
			if (!c->active) {
				if(!c->isbubble){
					map->turnOnCore(tasks[count], temp);
					tasks.at(count).startRunning(temp);
					c->setFreq(1+(double)(region.area - taskNum)/(taskNum*1.5));
					count++; //map one more task 
				}	
			}
			if(count == tasks.size())
							break;
		}
	}	
	if (count == tasks.size()) {		
		return true;
		//all tasks have benn mapped already
	}
	else {/*unhook map*/
		unhookFromMap(map);
		cout<<" map failed" <<endl;
		return false;
		}
} 


bool Event::localColdest(Map* map, vector<int> hot_tid){
	if(this->region.area >= 2*tasks.size())
	{	if(squareMigration(map))
		 	return true;
		else
			return false;
	}
	/*Find coolest core and migrate*/
	cout<<"do local coolest Migration for app"<<getId()<<endl;
	
	double coldest_tmp = threshold;
	Vertex  previous_vertex;
	Vertex coldest_vertex;
	for (auto tid : hot_tid) {
		// cout <<"Hot task:"<<tid<<endl;
		previous_vertex = tasks[tid].getLandlord();
		map->turnOffCore(tasks[tid].getLandlord());
		tasks[tid].release2();
		for(int j = region.start_p.l; j <= region.end_p.l; j++) {
			for (int k = region.start_p.w; k <= region.end_p.w; k++){
				// if (!(k==4 && (j==2|| j==3)))
				// {
					/* add 2 more bubbles */
				
				Vertex temp(j, k);
				if(map->getSpecificCore(temp)->getTaskId() == -1){
					double tmp_cur = map->getSpecificCore(temp)->temperture;
					if ( tmp_cur < coldest_tmp)
					{
						coldest_tmp = tmp_cur;
						coldest_vertex = temp;
					}
				}
				// }
			}
		}

		if (coldest_vertex.w == -1)
		{
			/*decrease the frequency, nowhere to migrate*/
			map->turnOnCore(tasks[tid], previous_vertex);
			map->getSpecificCore(previous_vertex)->setFreq(0.5);
			// cout<<"decrease the frequency, nowhere to migrate"<<endl;
			tasks[tid].continueRunning(previous_vertex);
		}
		else{
			/*add cost to total migrate distance*/
			migrate_distance += coldest_vertex.Manhattan(previous_vertex);
			map->turnOnCore(tasks[tid], coldest_vertex);
			tasks[tid].continueRunning(coldest_vertex);
			// cout<<"migrate to dark core"<<endl;
			map->getSpecificCore(coldest_vertex)->setFreq(1+(double)(region.area - taskNum)/(taskNum*1.5));
			coldest_tmp = threshold;/*Find next coldest core*/
			coldest_vertex.w = -1;
		}
		
	}	
	migratetimes++;		
	return true;
}

bool Event::localNeighbor(Map* map, vector<int> hot_tid){
	/*Find adjacent coolest core in its region and migrate*/
	cout<<"do localNeighbor Migration for app"<<getId()<<endl;
	
	double coldest_tmp = threshold;
	Vertex  previous_vertex;
	Vertex coldest_neighbor;
	for (auto tid : hot_tid) {
		// cout <<"Hot task:"<<tid<<endl;
		previous_vertex = tasks[tid].getLandlord();
		map->turnOffCore(tasks[tid].getLandlord());
		tasks[tid].release2();
		for(int k = previous_vertex.l+1; k >= previous_vertex.l-1; k--) {
			for (int j = previous_vertex.w+1; j >= previous_vertex.w-1; j--) {
				if(k >= region.start_p.l && k <= region.end_p.l && j >=region.start_p.w && j<= region.end_p.w)
				{	
					Vertex temp(k, j);
					Core* c = map->getSpecificCore(temp);
					if(c->getTaskId() == -1) 
					{
						double tmp_cur = map->getSpecificCore(temp)->temperture;
						if ( tmp_cur < coldest_tmp)
						{
							coldest_tmp = tmp_cur;
							coldest_neighbor = temp;
						}
					}
				}
			}
		}
		if (coldest_neighbor.w == -1)
		{
			/*decrease the frequency, nowhere to migrate*/
			map->turnOnCore(tasks[tid], previous_vertex);
			map->getSpecificCore(previous_vertex)->setFreq(0.5);
			tasks[tid].continueRunning(previous_vertex);
			// cout<<"decrease the frequency, nowhere to migrate"<<endl;
		}
		else{
			/*add cost to total migrate distance*/
			migrate_distance += coldest_neighbor.Manhattan(previous_vertex);
			if(map->getSpecificCore(coldest_neighbor)->getTaskId() == -1){
				map->turnOnCore(tasks[tid], coldest_neighbor);
				map->getSpecificCore(coldest_neighbor)->setFreq(1+(double)(region.area - taskNum)/(taskNum*1.5));
			
				tasks[tid].continueRunning(coldest_neighbor);
			}
			
			else
			{
				tasks[tid].continueRunning(previous_vertex);
				InvokedTask* t1 = &tasks[map->getSpecificCore(coldest_neighbor)->getTaskId()] ;
				InvokedTask* t2 = &tasks[tid];
				swapTask(map , t1, t2);
			}
			
			
			// cout<<"migrate to dark core"<<endl;
			coldest_tmp = threshold ;/*Find next coldest core*/
			coldest_neighbor.w = -1;
		}
		
	}
		
	migratetimes++;		
	return true;
}

bool Event::dvfs(Map* map, vector<int> hot_tid){
	cout<<"do dvfs for app"<<getId()<<endl;
	for (auto tid : hot_tid) {
		cout <<"Hot task:"<<tid<<endl;
		map->getSpecificCore(tasks[tid].getLandlord())->setFreq(0.6);
	}
	return true;
}