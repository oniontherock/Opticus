#include "TimeHandler.hpp"


uint32_t TimeHandler::timeRealPrev{ uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
uint32_t TimeHandler::timeRealCur{ uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
uint32_t TimeHandler::timeSimulatedPrev{ uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };
uint32_t TimeHandler::timeSimulatedCur{ uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) };

double TimeHandler::deltaReal{ 0.0 };
double TimeHandler::deltaSimulated{ 0.0 };

float TimeHandler::timescale{ 1.f };


void TimeHandler::deltaCompute() {

	using namespace std::chrono;

	timeRealCur = uint32_t(duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count());
	uint32_t timeDiffReal = timeRealCur - timeRealPrev;
	timeRealPrev = timeRealCur;

	deltaReal = float(timeDiffReal) / 1000.f;

	// the simulated time movement in milliseconds since last frame.
	// timeSimulatedCur + simulatedTimeMove is equal to the current time according to the timescale
	uint32_t simulatedTimeMovement = uint32_t((timeDiffReal * (timescale * 1000)) / 1000);

	timeSimulatedCur += simulatedTimeMovement;
	uint32_t timeDiffSimulated = timeSimulatedCur - timeSimulatedPrev;
	timeSimulatedPrev = timeSimulatedCur;

	deltaSimulated = float(timeDiffSimulated) / 1000.f;
}
double TimeHandler::deltaRealGet() {
	return deltaReal;
}
double TimeHandler::deltaSimulatedGet() {
	return deltaSimulated;
}

uint32_t TimeHandler::timeRealGet() {
	return timeRealCur;
}
uint32_t TimeHandler::timeSimulatedGet() {
	return timeSimulatedCur;
}
