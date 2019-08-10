
#include "Clock.h"

Clock* Clock::clock = new Clock;

Clock::Clock() {
	count = 0;
}

Clock* Clock::getClock() {
	if (clock == nullptr) {
		clock = new Clock;
	}
	return clock;
}
