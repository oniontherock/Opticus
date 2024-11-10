#ifndef __AI_STATES_H__
#define __AI_STATES_H__

#include "../../../../Common/Cooldown.hpp"
#include "../UtilityState.hpp"
#include <ECS/Entities/Entity.hpp>

namespace UtilityStates {
	struct StateIdle : StateBase {
		UtilityStateScore  condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) override;
		void behavior(Entity& actor, const ActorBlackboard& actorBlackboard) override;
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new StateIdle());
		};
	};
	struct StateActorFollow : StateBase {
		UtilityStateScore  condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) override;
		void behavior(Entity& actor, const ActorBlackboard& actorBlackboard) override;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new StateActorFollow());
		};
	};
	struct StatePointGoTo: StateBase {
		UtilityStateScore  condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) override;
		void behavior(Entity& actor, const ActorBlackboard& actorBlackboard) override;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new StatePointGoTo());
		};
	};
}


#endif