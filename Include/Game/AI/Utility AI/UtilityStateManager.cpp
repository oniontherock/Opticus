#include "UtilityStateManager.hpp"
#include <algorithm>

UtilityStateManager::UtilityStateManager() {
	stateActiveInd = 0;
}
UtilityStateManager::UtilityStateManager(std::vector<UtilityStates::StateBase*> _statesVector) {
	for (uint16_t i = 0; i < _statesVector.size(); i++) {
		statesVector.push_back(StateSharedPtr(_statesVector[i]));
	}

	stateScores.resize(statesVector.size());

	stateActiveInd = 0;
}

UtilityStateManager::UtilityStateManager(UtilityStateManager& other) {
	for (uint16_t i = 0; i < other.statesVector.size(); i++) {
		statesVector.push_back(StateSharedPtr(static_cast<UtilityStates::StateBase*>(Duplicatable::duplicateAndGetRaw(other.statesVector[i].get()))));
	}

	stateScores.resize(statesVector.size());

	stateActiveInd = other.stateActiveInd;
}
UtilityStateManager::UtilityStateManager(const UtilityStateManager& other) {
	for (uint16_t i = 0; i < other.statesVector.size(); i++) {
		statesVector.push_back(StateSharedPtr(static_cast<UtilityStates::StateBase*>(Duplicatable::duplicateAndGetRaw(other.statesVector[i].get()))));
	}

	stateScores.resize(statesVector.size());

	stateActiveInd = other.stateActiveInd;
}
void UtilityStateManager::operator= (UtilityStateManager& other) {
	for (uint16_t i = 0; i < other.statesVector.size(); i++) {
		statesVector.push_back(StateSharedPtr(static_cast<UtilityStates::StateBase*>(Duplicatable::duplicateAndGetRaw(other.statesVector[i].get()))));
	}

	stateScores.resize(statesVector.size());

	stateActiveInd = other.stateActiveInd;
}
void UtilityStateManager::operator= (const UtilityStateManager& other) {
	for (uint16_t i = 0; i < other.statesVector.size(); i++) {
		statesVector.push_back(StateSharedPtr(static_cast<UtilityStates::StateBase*>(Duplicatable::duplicateAndGetRaw(other.statesVector[i].get()))));
	}

	stateScores.resize(statesVector.size());

	stateActiveInd = other.stateActiveInd;
}


void UtilityStateManager::statesEvaluate(const Entity& actor, const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	for (uint16_t i = 0; i < statesVector.size(); i++) {
		stateScores[i] = statesVector[i]->condition(actor, actorData, actorBlackboard);
	}
}
void UtilityStateManager::statesSort() {
	for (uint16_t i = 0; i < statesVector.size(); ++i) {
		for (uint16_t j = 0; j < statesVector.size(); ++j) {
		

			if (stateScores[i] > stateScores[j]) {
				statesVector[i].swap(statesVector[j]);
				std::swap(stateScores[i], stateScores[j]);
			}
		}
	}
}

void UtilityStateManager::statesUpdate(const Entity& actor, const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	statesEvaluate(actor, actorData, actorBlackboard);
	statesSort();
}

void UtilityStateManager::stateActiveUpdate(Entity& actor, ActorBlackboard& actorBlackboard) {
	statesVector[stateActiveInd]->behavior(actor, actorBlackboard);
}
void UtilityStateManager::stateActiveSet(uint16_t ind) {
	stateActiveInd = ind;
}

StateVector UtilityStateManager::statesGet() {
	return statesVector;
}
uint16_t UtilityStateManager::statesCountGet() {
	return uint16_t(statesVector.size());
}
std::vector<UtilityStates::StateBase*> UtilityStateManager::statesGetBeyondThreshold(UtilityStateScore threshold) {

	// vector of states beyond threshold
	std::vector<UtilityStates::StateBase*> states;

	for (uint16_t i = 0; i < statesVector.size(); i++) {
		// we break at the first score below the threshold because the statesVector is sorted by score,
		// therefore, any state after a state that is too low will be below the threshold
		if (stateScores[i] < threshold) break;

		states.push_back(statesVector[i].get());
	}

	return states;
}

