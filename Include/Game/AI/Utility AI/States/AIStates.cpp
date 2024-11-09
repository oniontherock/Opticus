#include "../../../ACECS/ECSRegistry.hpp"
#include "AIStates.hpp"

using namespace UtilityStates;
using namespace EntityComponents;
using namespace EntityEvents;

UtilityStateScore StateIdle::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = 10;

	return score;
}
void StateIdle::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {
}

UtilityStateScore StateFollowPlayer::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = actorBlackboard.dataHas("PlayerId") ? 50 : 0;

	return score;
}
void StateFollowPlayer::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {

	if (!actorBlackboard.dataHas("PlayerId")) return;

	sf::Vector2f playerPosition = actorBlackboard.dataGet<sf::Vector2f>("PlayerPosition");

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = playerPosition;
	eventGoTo->desiredDist = actorBlackboard.dataGet<float>("DesiredDistanceToTarget");

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = playerPosition;
}



