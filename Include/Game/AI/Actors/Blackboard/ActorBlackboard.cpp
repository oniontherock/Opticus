#include "ActorBlackboard.hpp"



void ActorBlackboard::dataRemove(const BlackboardDataName& dataName) {
	blackboardDataMap.erase(blackboardDataMap.begin());
}


