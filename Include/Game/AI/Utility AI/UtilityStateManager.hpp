#ifndef __UTILITY_STATE_MANAGER_H__
#define __UTILITY_STATE_MANAGER_H__

#include "UtilityState.hpp"
#include <cstdint>
#include <ECS/Entities/Entity.hpp>
#include <memory>
#include <vector>

// shared_ptr to a StateBase, note that this is generally used in a polymorphic way
typedef std::shared_ptr<UtilityStates::StateBase> StateSharedPtr;
// vector of StateSharedPtr
typedef std::vector<StateSharedPtr> StateVector;

// contains a vector of UtilityStates,
// updates and sorts states based off of score.
class UtilityStateManager {

	// vector of states that this UtilityStateManager contains,
	// sorted by state score.
	StateVector statesVector;
	// vector of scores for each state in the statesVector
	std::vector<UtilityStateScore> stateScores;

	// index of the active state
	uint16_t stateActiveInd;

	// calls the condition function on every state in the statesVector.
	void statesEvaluate(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard);
	// sort the statesVector based off of the score of the states.
	void statesSort();


public:

	UtilityStateManager();
	UtilityStateManager(std::vector<UtilityStates::StateBase*> _statesVector);

	// evaluate and sort the scores of every state.
	void statesUpdate(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard);
	
	// calls the behavior function on the active state
	void stateActiveUpdate(Entity& actor, const ActorBlackboard& actorBlackboard);
	// sets the active state to be the
	void stateActiveSet(uint16_t ind);

	// returns a reference to the statesVector
	StateVector statesGet();
	// gets the number of states this UtilityStateManager contains
	uint16_t statesCountGet();
	// returns the states in the statesVector that have a score higher than the threshold,
	std::vector<UtilityStates::StateBase*> statesGetBeyondThreshold(UtilityStateScore threshold);
};


#endif