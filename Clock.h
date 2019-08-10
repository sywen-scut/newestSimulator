#pragma once

/* Singleton Clock*/

class Clock {
private:
	int count;
	Clock();

	/*clock object*/
	static Clock* clock;
public:
	static Clock* getClock();

	/*add N clock cycle*/
	inline void tick(int n = 1) {
		count += n;
	}

	/*return current cycle*/
	inline int now() {
		return count;
	}

};


 /*	Lesson learned
	the singleton pattern in C++ need "static" attibute, which initialized outside of the class.
	static 
	thread security

 */