#ifndef __UTILITY_STATE_H__
#define __UTILITY_STATE_H__

#include "../Actors/Blackboard/ActorBlackboard.hpp"
#include "../Actors/Data/ActorDataHolder.hpp"
#include <cstdint>
#include <memory>
#include <ECS/Duplicatable/Duplicatable.hpp>
#include <ECS/Entities/Entity.hpp>
#include <ECS/TypeDefinitions.hpp>

typedef uint8_t UtilityStateScore;

namespace UtilityStates {
	// base class for AI states,
	struct StateBase : Duplicatable {

		// determines the score based off of the actors data and blackboard
		virtual UtilityStateScore condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) = 0;
		// performs certain behaviors depending on the state,
		// uses the actor for things like sending move events
		// uses the blackboard for things like finding the location of enemies in view
		virtual void behavior(Entity& actor, const ActorBlackboard& actorBlackboard) = 0;
	};
}


#endif