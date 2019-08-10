#include "Map.h"
	
	vector<Core> Map::initCores(double Freq, int width, int length){
		vector<Core> cores;
		for (int l = 0; l < length; l++) {
		
			for (int w = 0; w < width; w++) {
				Core core(Freq, w+ l * width);
				cores.push_back(core);
			}
		}
	
		return cores;
	}

	Map::Map(double runFreq, int width, int length){
		cores = initCores(runFreq, width, length);
		this->width = width;
		this->length = length;
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

	bool Map::turnOnCore(InvokedTask task,Vertex v){
		if (!getSpecificCore(v)->active)
		{
			getSpecificCore(v)->turnOn(task.getApplicationId(), task.getId(), task.getIpc());
			return true;
		}
		return false;
	}

	bool Map::turnOffCore(Vertex v){
		if (getSpecificCore(v)->active)
		{
			getSpecificCore(v)->turnOff();
			return true;
		}
		return false;

	}

	void Map::showMap(){
		cout << endl;
		for(int k = 0; k < length; k++) {
				for (int j = 0; j < width; j++) {
					Vertex temp(k, j);
					
					
					if (!cores.at(temp.toIndex()).active) 
						cout <<"x"<< " ";
					else
						cout <<cores.at(temp.toIndex()).getTaskId() << " ";

					// if (!cores.at(temp.toIndex()).occupied) 
					// 	cout <<"x"<< " "; 
					// else
					// 	if (cores.at(temp.toIndex()).getTaskId() == -1) 
					// 		cout <<"o"<< " ";
					// 	else
					// 		cout <<cores.at(temp.toIndex()).getTaskId() << " ";
				}
			cout << endl;
		}
		cout << endl;
	}


	void Map::writePtrace(FILE* fp)
	{
		double power_cur;
		Core* c;
		
		for (int k = 0; k < length; k++)
		{
			for (int j = 0; j < width; j++) {
					Vertex temp(k, j);
					c = & cores.at(temp.toIndex());
					power_cur = c->p_power_leakage+ c->p_power_dynamic+c->r_power;
					fprintf(fp, "%f ", power_cur*18.09);
					//18.09
				}
		}
		fprintf(fp, "\n");
	}

	void Map::getTemperature(string path)
	{
		Core *c;
		FILE *fp = fopen(path.c_str(), "r");
		if (fp == NULL)
		{
			cout<<"open failed"<<endl;
			exit(0);
		}
		float readtmp;
		for (int k = 0; k < length; ++k)
		{
			for (int j = 0; j < width; ++j)
			{
				Vertex temp(k,j);
				fscanf(fp, "%f ",&readtmp);
				c = & cores.at(temp.toIndex());
				c->temperture = readtmp;
			}
		}
		fclose(fp);
	}

	