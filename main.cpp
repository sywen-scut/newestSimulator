#include <iostream>
#include <string>
#include <queue>
#include <stdlib.h>  
#include <algorithm>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include "global.h"
#include "Event.h"


double threshold = 55;

using namespace std;

queue<Event> readEventFromFile(int numApplication, string filenamePrefix) {
	queue<Event> results;
	for (int i = 0; i < numApplication; i++) {
		Application app(filenamePrefix + to_string(i) + ".txt");
		Event e(app);
		results.push(e);

	}           

	return results;
}

/*check if temperture exceed threshold*/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
bool CheckTemperture(Map* map){
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

/*check which application and which tasks exceed threshold*/
vector<vector<int>> CheckHotApps(Map* map){
	vector<vector<int>> hot_aid_tid;
	vector<int> hot_aid;
	vector<int> hot_tid;
	Core* core_temp;
	for(int k = 0; k < map->getLength(); k++) {
		for (int j = 0; j < map-> getWidth(); j++) {
				Vertex temp(k, j);
				core_temp = map->getSpecificCore(temp);
				if (core_temp->active)
				{
					if(core_temp->temperture > threshold){
						hot_aid.push_back(core_temp->getApplicationId());
						hot_tid.push_back(core_temp->getTaskId());
					}

				}
		}
	}
	hot_aid_tid.push_back(hot_aid);
	hot_aid_tid.push_back(hot_tid);	
	return hot_aid_tid;
}
extern int netsize;
int main(int argc,char* argv[]) {
	
	
	int numApplication = 2;
	queue<Event> events;
	
	// double ccr = atof(argv[5]);
	// for (int i =1; i<= numApplication; i++)
	// {
	// 	// int nT,double r, int aT, int i,int taskVol,int type,int fixedData
	// 	Application temp_app(appSize, ccr, 0, i, 500, 3, 0);
	// 	temp_app.saveAsFile("../simulator/IPS/type"+to_string(3)+"app",0);
	// 	Event temp_e(temp_app);
	// 	events.push(temp_e);
	// }
	/*string app_filepath = "../simulator/app/single/level2.txt";
	Event t1(app_filepath);
	events.push(t1);*/
	// string ar = argv[3];

	// Application testApp1("../simulator/app/arrivalrate/"+ar+"/t1.txt");
	// Event t1(testApp1);
	// events.push(t1);
	
	if (argc != 6 ) {
		cout<<"Wrong input!"<<endl;
		cout<<"Select migrationPattern, interval,commvol, networksize"<<endl;
		return 0;
	}
	
	
	int t_interval = atoi(argv[2]);
	global g(atoi(argv[4]));
	int commvol = atoi(argv[3]);
	double CCR = (double(commvol))/500;
	string realbench = argv[5];
	/** task comp vol 500**/
	/* Application testApp1(CCR, 0, 1, 500, edgefile_path);
	Event t1(testApp1);
	events.push(t1); */

	/** task comp vol 500**/
	//ccr; arrive time; id; taskVol; edgefile
	// Application app0(CCR, 0, 0, 500, edgefile_path);
	// Application app1(CCR, 0, 1, 500, edgefile_path);
	// Application app2(CCR, 0, 2, 500, edgefile_path);
	// Application app3(CCR, 0, 3, 500, edgefile_path);
	// // Application app4(CCR, 2000, 4, 500, edgefile_path);
	// // Application app5(CCR, 2000, 5, 500, edgefile_path);
	// // Application app6(CCR, 2000, 6, 500, edgefile_path);
	// // Application app7(CCR, 2000, 7, 500, edgefile_path);

	// Event t0(app0);
	// events.push(t0);
	// Event t1(app1);
	// events.push(t1);
	// Event t2(app2);
	// events.push(t2);
	// Event t3(app3);
	// events.push(t3);
	// Event t4(app4);
	// events.push(t4);
	// Event t5(app5);
	// events.push(t5);
	// Event t6(app6);
	// events.push(t6);
	// Event t7(app7);
	// events.push(t7);
    // banres blackscholes canneal dedup ferret fluidanimate freqmine raytrace streamcluster swaptions
	/* realbench.push_back("banres");
	realbench.push_back("blackscholes");
	realbench.push_back("canneal");
	realbench.push_back("dedup");
	realbench.push_back("ferret");
	realbench.push_back("fluidanimate");
	realbench.push_back("freqmine");
	realbench.push_back("raytrace");
	realbench.push_back("streamcluster");
	realbench.push_back("swaption"); */
	
	/* for (int i = 0; i < numApplication; i++) {
		string app_path = "../simulator/app/old/"+realbench[i+1]+"0.txt";
	
		Application app(app_path);
		app.saveAsFile("../simulator/app/old/1",0);
		Event e(app);
		events.push(e);

	}  */     
	string app_path = "../simulator/app/old/"+realbench+"0.txt";
	Application app(app_path);
	app.saveAsFile("../simulator/app/old/1",0);
	Event e(app);
	events.push(e);
	events.push(e);

	double commCost = 0;

	Clock* clock = Clock::getClock();
	/*Running frequency 1, mesh size 8*8*/
	Map map(1, netsize, netsize);
	

	string type_str = argv[1];
	char buf[256], basename[128];
	sprintf(basename, "%s_%s_%s_%s_%s", argv[1], argv[2],argv[3], argv[4],argv[5]);
	sprintf(buf, "Ptrace/%s.ptrace", basename);
	string Ptrace_filepath = buf;
	FILE* fp = fopen(Ptrace_filepath.c_str(),"w");
	if( fp==NULL )
		{cout<<"can't find output path"<<endl;
		exit(0);
	}
	else
		cout << "Output: " <<Ptrace_filepath << endl;
	for (int i = 0; i < netsize; i++)
	{
		for (int j = 0; j < netsize; j++)
		{
			fprintf(fp, "P_%d_%d \t", j, i);
		}

	}
	fprintf(fp, "\n");
	fclose(fp);

	int type = atoi(argv[1]);
	int regsize = 98;


	
	/* switch (netsize)
	{
		case 8:
			regsize = 12;
			break;
		case 10:
			regsize = 100;
			break;
		case 12:
			regsize = 121;
			break;
		case 14:
			regsize = 98;
			break;
		default:
			break;
	} */
	// FILE* rfp = fopen("../simulator/Result","a+");
	// fprintf(rfp, "type %d %s\n", type, ar.c_str());
	// fclose(rfp);
	if(type == 4)
		sprintf(buf, "../simulator/execost/localcoolest.log");
	if(type == 3)
		sprintf(buf, "../simulator/execost/globalcoolest.log");
	if(type == 6)
		sprintf(buf, "../simulator/execost/neighborcoolest.log");
	
	/*CoolestEdge CoolestCenter  NeighborEdge NeighborCenter*/
	/* if(type == 4)
		sprintf(buf, "../simulator/execost/NeighborEdge.log");
	if(type == 5)
		sprintf(buf, "../simulator/execost/NeighborCenter.log"); */
	string runlog = buf;

	char hotspot_cmd[256];
	sprintf(hotspot_cmd, "../HotSpot-6.0/simulator%d.sh %s",netsize, basename);
	printf("../HotSpot-6.0/simulator%d.sh %s\n",netsize, basename);
	
	sprintf(buf, "../HotSpot-6.0/%s.ttrace", basename);
	string ttrace_path = buf;
	
	cout<<"start running"<<endl;
	vector<Event> waiting;
	vector<Event> running;
	vector<Event> finished;
	/*1 square 2 chess 3 nonContigous 4 with bubble 6 neighbourcoolest*/
	

	Vertex cur_p(0,0);
	Vertex lefttop(0, 0);
	Map* map_t = &map;

	double sum_IPS = 0;
	double sum_DTS = 0;   

	while (finished.size() != numApplication) {
		
		for (vector<Event>::iterator iter = waiting.begin(); (!waiting.empty())&&iter != waiting.end();) 
		{
			if ((*iter).startRunning(&map, type, cur_p, regsize)) {
				cout<<"time : "<< clock->now() <<endl;
				map.showMap();
				switch (type){
				case 1:
					if ((*iter).region.end_p.w<3)
						cur_p.w = 4;
					else
						cur_p.w = (*iter).region.end_p.w + 1;

					if(!map_t->getSpecificCore(lefttop)->occupied)
						cur_p = lefttop;
					break;
				case 2:
					if ((*iter).region.end_p.l<3)
						cur_p.l = 4;
					else
						cur_p.l = (*iter).region.end_p.l + 1;
						
					if(!map_t->getSpecificCore(lefttop)->occupied)
						cur_p = lefttop;
					break;
				case 3:case 6:
					break;
				case 5:{
						int chaos = 0;
						for(int k = 0; k < netsize ; k+=4) {
								for (int j = 0; j < netsize; j+=4) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
				}
				break;
				case 4: 
					{
					int chaos = 0;
						
						/* if (regsize == 100) {
							 for(int k = 0; k < map.getLength() ; k+=10) {
								for (int j = 0; j < map.getWidth(); j+=10) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						} */
						if (regsize == 98) {
							if(!map_t->getSpecificCore(cur_p)->occupied)
							{
									Vertex temp(7, 0);
									cur_p = temp;
									
							}else
										break;
							}
						
						if (regsize == 16) {
							 for(int k = 0; k < map.getLength() ; k+=4) {
								for (int j = 0; j < map.getWidth(); j+=4) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 12) {
							 for(int k = 0; k < 8 ; k+=3) {
								for (int j = 0; j < 8; j+=4) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						/* if (regsize == 25) {
							 for(int k = 0; k < map.getLength() ; k+=5) {
								for (int j = 0; j < map.getWidth(); j+=5) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 32) {
							 for(int k = 0; k < map.getLength() ; k+=8) {
								for (int j = 0; j < map.getWidth(); j+=4) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						} */
					}
						break;
				default:
					break;
				}

				
				running.push_back(*iter);
				iter = waiting.erase(iter);
			}
			else{
				iter++;
			}
		}

		while (!events.empty()) {
			Event app = events.front();
			if (app.arrive(clock->now())) {
				if (app.startRunning(&map, type, cur_p,regsize)) {

					cout<<"time : "<< clock->now() <<endl;
					map.showMap();
					switch (type){
					case 1:
						if (app.region.end_p.w<3)
							cur_p.w = 4;
						else
							cur_p.w = app.region.end_p.w + 1;
						break;
					case 2:
						if (app.region.end_p.l<3)
							cur_p.l = 4;
						else
							cur_p.l = app.region.end_p.l + 1;
						break;
					case 3: case 6:
						break;
					case 5:{
						int chaos = 0;
						for(int k = 0; k < netsize ; k+= 7) {
								for (int j = 0; j < netsize; j+=14) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
				}
				break;
					case 4:
						{
						int chaos = 0;
						if (regsize == 98) 
						{	Vertex temp(7, 0);
							cur_p = temp;
						}	
						/* if (regsize == 100) {
							 for(int k = 0; k < map.getLength() ; k+=10) {
								for (int j = 0; j < map.getWidth(); j+=10) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 121) {
							 for(int k = 0; k < map.getLength() ; k+=12) {
								for (int j = 0; j < map.getWidth(); j+=12) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 98) {
							 for(int k = 0; k < map.getLength() ; k+=14) {
								for (int j = 0; j < map.getWidth(); j+=7) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 8) {
							 for(int k = 0; k < 8 ; k+=2) {
								for (int j = 0; j < 8; j+=2) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}

						if (regsize == 9) {
							 for(int k = 0; k < 8 ; k+=3) {
								for (int j = 0; j < 8; j+=3) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 10) {
							 for(int k = 0; k < 8 ; k+=2) {
								for (int j = 0; j < 8; j+=5) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}*/
						if (regsize == 12) {
							 for(int k = 0; k < netsize ; k+=3) {
								for (int j = 0; j < netsize; j+=4) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
					/*	if (regsize == 14) {
							 for(int k = 0; k < 8 ; k+=2) {
								for (int j = 0; j < 7; j+=7) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 15) {
							 for(int k = 0; k < 8 ; k+=3) {
    							for (int j = 0; j < 8; j+=5) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						} */
						if (regsize == 16) {
							 for(int k = 0; k < netsize ; k+=4) {
								for (int j = 0; j < netsize; j+=4) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						/* if (regsize == 18) {
							 for(int k = 0; k < 8 ; k+=4) {
								for (int j = 0; j < 8; j+=4) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 20) {
							 for(int k = 0; k < 8 ; k+=4) {
								for (int j = 0; j < 8; j+=5) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						}
						if (regsize == 25) {
							 for(int k = 0; k < map.getLength() ; k+=5) {
								for (int j = 0; j < map.getWidth(); j+=5) {
								Vertex temp(k, j);
								if(!map_t->getSpecificCore(temp)->occupied){
									cur_p = temp;
									chaos = 1;
								}if (chaos == 1)
									break;
								}if (chaos == 1)
										break;
							}
						} */

					}
						break;
					default:
						break;
					}
					running.push_back(app);
					events.pop();
					
				}
				else {
					app.wait();
					waiting.push_back(app);
					events.pop();
				}
			}
			else {
				break;
			}

		}
		
		// FILE *ifp = fopen(IPS_filepath.c_str(),"a+");
		for (vector<Event>::iterator iter = running.begin(); iter != running.end();) {
			(*iter).nRound(&map);
			sum_DTS += iter->inst_cur;
			sum_IPS += iter->trans_cur;
			if ((*iter).hasFinished()) {
				cur_p = (*iter).region.start_p;
				finished.push_back(*iter);
				iter = running.erase(iter);
			}
			else {
				iter++;
			}
		}
		// fprintf(ifp, "%d\t%f\n", clock->now(),sum_IPS+sum_DTS);
		// fclose(ifp);
		
		/*write power trace*/
		fp = fopen(Ptrace_filepath.c_str(),"a+");
		map.writePtrace(fp);
		fclose(fp);

		/*check temperature and migrate tasks here*/
		if (clock->now() % t_interval == 0 && clock->now() !=0)
		{
			system(hotspot_cmd);
			map.getTemperature(ttrace_path);
			
			if(CheckTemperture(&map)){
				cout<<"time : "<< clock->now() <<endl;
				// tasks = squareMigration(map_t, tasks);
				vector<vector<int>> hot_aid_tid = CheckHotApps(&map);
				vector<int> hot_aid = hot_aid_tid[0];
				sort(hot_aid.begin(), hot_aid.end());
				vector<int>::iterator aiter = unique(hot_aid.begin(), hot_aid.end());
				hot_aid.erase(aiter, hot_aid.end());
				// for(auto aid:hot_aid_tid[0]){
				// 	cout<<aid<<'\t';
				// }
				// cout<<endl;
				// for(auto aid:hot_aid){
				// 	cout<<aid<<'\t';
				// }
				// cout<<endl;
				for (auto aid:hot_aid)
				{
					for (vector<Event>::iterator iter = running.begin(); iter != running.end();){
						if ((*iter).getId() == aid)
						{
							vector<int> cur_hot_tid;
							int index = 0;
							for (auto tid:hot_aid_tid[1]){
								if (hot_aid_tid[0].at(index) == (*iter).getId())
								{
								cur_hot_tid.push_back(tid);
								}
								index++;
							}
							switch(type){
								case 1:
									if((*iter).squareMigration(&map))
										cout<<"migrate succcessfully"<<endl;
									else
										cout<<"migrate failed"<<endl;
									break;
								case 2:
									if((*iter).chessMigration(&map))
										cout<<endl;
									else
										cout<<"migrate failed"<<endl;
									break;
								case 3:{
									
									if((*iter).globalColdest(&map, cur_hot_tid))
										cout<<"migrate succcessfully"<<endl;
									else
										cout<<"migrate failed"<<endl;
								}
									break;

								case 4:case 5:{
									vector<int> cur_hot_tid2;
									int index2 = 0;
									cout<<"Hot tasks id:"<<'\t';
									for (auto tid:hot_aid_tid[1]){
										if (hot_aid_tid[0].at(index2) == (*iter).getId())
										{
											cur_hot_tid2.push_back(tid);
											cout<<tid<<'\t';
										}
										index2++;
									}
									cout<<endl;
									if((*iter).CCR > 0){

										if(!(*iter).localColdest(&map, cur_hot_tid2))
											cout<<"migrate failed"<<endl;
									}
									else{
										if(!(*iter).localNeighbor(&map, cur_hot_tid2))
											cout<<"migrate failed"<<endl;
									}
								}
									break;
								/* case 5:{
									vector<int> cur_hot_tid3;
									int index3 = 0;
									for (auto tid:hot_aid_tid[1]){
										if (hot_aid_tid[0].at(index3) == (*iter).getId())
										{
											cur_hot_tid3.push_back(tid);
										}
										index3++;
									}
									if((*iter).dvfs(&map, cur_hot_tid3))
										cout<<"low frequency succcessfully"<<endl;
									else
										cout<<"DVFS failed"<<endl;
								}
									break; */
								case 6:{
									for (int j = 0; j < 8; j++) {
										for (int k = 0; k < 8; k++) {
												Vertex temp(j, k);
												map_t->getSpecificCore(temp)->unoccupy();
										}
									}
									if((*iter).neighborCol(&map, cur_hot_tid))
										cout<<"migrate succcessfully"<<endl;
									else
										cout<<"migrate failed"<<endl;
									
								}
									break;
							}
						}
						iter++;
					}
				}	
				map.showMap();
			}
		}
		clock->tick();
	}

	cout << "total running time: " << clock->now() << endl;
	FILE *ifp = fopen(runlog.c_str(),"a+");
	/* for(auto fapp:finished){
		fprintf(ifp, "appid: %d\twaiting: %d\texecution: %d\nresponse: %d\n",fapp.getId(),fapp.getStartTime()-fapp.getArrivalTime(),fapp.getFinishTime()-fapp.getStartTime(),fapp.getFinishTime()-fapp.getArrivalTime());
		fprintf(ifp, "taskNum: %d\tcommvol: %d\tcommcost: %d\n",fapp.getNumTasks(),commvol,fapp.getFinishTime()-fapp.getStartTime());
		
	} */
	fprintf(ifp, "%s\t%d\n", realbench.c_str(),clock->now());
	fclose(ifp);
	
	map.showMap();
	return 0;
}
