#include "Core.h"

	Core::Core(double runFreq, int index){
		active = false;
		occupied = false;
		this->Freq = runFreq;
		Vertex v(floor(index / 8), index% 8);
		this->location = v;
		application_id = -1;
		task_id = -1;
		task_ipc = 0;

	}

	void Core::setFreq(double freq){
		if(freq > 2)
			this->Freq = 2;
		else
			this->Freq = freq;
	}


	/*turn on the core running specific task*/
	void Core::turnOn(int a_id, int t_id, double t_ipc){
		application_id = a_id;
		task_id = t_id;
		task_ipc = t_ipc;
		active = true;
		
	}

	/*turn off the core*/
	void Core::turnOff(){
		application_id = -1;
		task_id = -1;
		task_ipc = 0;
		active = false;
		p_power_dynamic = 0;
		r_power = 0;
	}

	void Core::turnOn_Processor(){
		p_power_dynamic = Freq*task_ipc;
	}

	void Core::turnOff_Processor(){
		p_power_dynamic = 0;
	}

	void Core::turnOn_router(){
		r_power = 0.1;
	}

	void Core::turnOff_router(){
		r_power = 0;
	}

	int Core::getApplicationId(){
		return application_id;
	}

	int Core::getTaskId(){
		return task_id;
	}
	
	double Core::getFreq(){
		return Freq;
	}

	void Core::occupy(){
		occupied = true;
	}

	void Core::unoccupy(){
	 	occupied = false;
	}

	void Core::setbubble(){
		isbubble = true;
	}
