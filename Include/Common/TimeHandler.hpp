#ifndef __DELTA_MANAGER_H__
#define __DELTA_MANAGER_H__

#include <chrono>

class TimeHandler {
	static uint32_t timeRealPrev;
	static uint32_t timeRealCur;
	static uint32_t timeSimulatedPrev;
	static uint32_t timeSimulatedCur;
	
	static double deltaReal;
	static double deltaSimulated;
public:

	static float timescale;

	// computes delta, should be done at the start of every frame.
	// computes both real and simulated delta
	static void deltaCompute();
	// gets the real (regardless of timescale) delta (time since last frame) in seconds.
	static double deltaRealGet();
	// gets the simulated (according to timescale) delta (time since last frame) in seconds.
	static double deltaSimulatedGet();

	static uint32_t timeRealGet();
	static uint32_t timeSimulatedGet();
};

#endif