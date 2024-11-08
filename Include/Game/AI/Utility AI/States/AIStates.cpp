#include "../../../ACECS/ECSRegistry.hpp"
#include "AIStates.hpp"

using namespace UtilityStates;

UtilityStateScore StateIdle::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = actorData.emotionGet(ActorEmotion::Fear) > 50 ? 0 : 25;

	return score;
}
void StateIdle::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {
}

UtilityStateScore StateWander::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = actorData.emotionGet(ActorEmotion::Fear) < 50 ? 0 : 25;

	return score;
}
void StateWander::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {
	auto* eventMove = actor.entityEventAddAndGet<EntityEvents::EventMove>();

	eventMove->moveAxis = sf::Vector2f(5, 0);
}



