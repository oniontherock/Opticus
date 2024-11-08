#ifndef __AI_STATES_H__
#define __AI_STATES_H__

#include "../UtilityState.hpp"
#include <ECS/Entities/Entity.hpp>

namespace UtilityStates {
	struct StateIdle : StateBase {
		UtilityStateScore  condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) override;
		void behavior(Entity& actor, const ActorBlackboard& actorBlackboard) override;
	};
	struct StateWander : StateBase {
		UtilityStateScore  condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) override;
		void behavior(Entity& actor, const ActorBlackboard& actorBlackboard) override;
	};
}


#endif