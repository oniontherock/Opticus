#include "ActorBlackboard.hpp"



void ActorBlackboard::dataRemove(BlackboardDataKey dataKey) {
	blackboardDataMap.erase(dataKey);
}

bool ActorBlackboard::dataHas(BlackboardDataKey dataKey) const {
	return blackboardDataMap.count(dataKey) != 0;
}
