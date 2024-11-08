#include "../../../ACECS/ECSRegistry.hpp"
#include "AIStates.hpp"

using namespace UtilityStates;
using namespace EntityComponents;
using namespace EntityEvents;

UtilityStateScore StateIdle::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = actorData.emotionGet(ActorEmotion::Fear) > 50 ? 0 : 25;

	return score;
}
void StateIdle::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {
	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();

	eventTurnTo->positionTo = actor.entityComponentGet<ComponentPosition>()->position + sf::Vector2f(10, 0);
}

UtilityStateScore StateWander::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = actorData.emotionGet(ActorEmotion::Fear) < 50 ? 0 : 25;

	return score;
}
void StateWander::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {
	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();

	eventGoTo->positionTo = actor.entityComponentGet<ComponentPosition>()->position + sf::Vector2f(10, 0);
}



