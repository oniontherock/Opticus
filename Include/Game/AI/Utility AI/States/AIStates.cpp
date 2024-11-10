#include "AIStates.hpp"
#include "../../../../Common/NumberGenerator.hpp"
#include "../../../ACECS/ECSRegistry.hpp"

using namespace UtilityStates;
using namespace EntityComponents;
using namespace EntityEvents;

UtilityStateScore StateIdle::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = 10;

	return score;
}
void StateIdle::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {
	
	if (!actorBlackboard.dataHas("PointWander")) return;

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = actorBlackboard.dataGet<sf::Vector2f>("PointWander");
	eventGoTo->desiredDist = 4.f;

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = actorBlackboard.dataGet<sf::Vector2f>("PointWander");
}

UtilityStateScore StateActorFollow::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = 0;

	if (actorBlackboard.dataHas("OrdersToSelf")) {
		OrderVector orders = actorBlackboard.dataGet<OrderVector>("OrdersToSelf");
		OrderSubVector& ordersActorFollow = orders[uint16_t(OrderType::ActorFollow)];

		if (ordersActorFollow.size() <= 0) goto noOrdersLabel;
		if (actorBlackboard.dataHas("LeaderId")) {
			for (uint16_t i = 0; i < ordersActorFollow.size(); i++) {
				if (ordersActorFollow[i].dataGet<EntityId>("OrderFromId") == actorBlackboard.dataGet<EntityId>("LeaderId")) {
					score = 75;
					break;
				}
			}
		}
	}
	// used when there are no orders to follow
	noOrdersLabel:

	return score;
}
void StateActorFollow::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {

	EntityId actorToFollowId = UINT32_MAX;

	if (actorBlackboard.dataHas("OrdersToSelf")) {
		OrderVector orders = actorBlackboard.dataGet<OrderVector>("OrdersToSelf");
		OrderSubVector& ordersActorFollow = orders[uint16_t(OrderType::ActorFollow)];

		if (actorBlackboard.dataHas("LeaderId")) {
			for (uint16_t i = 0; i < ordersActorFollow.size(); i++) {
				if (ordersActorFollow[i].dataGet<EntityId>("OrderFromId") == actorBlackboard.dataGet<EntityId>("LeaderId")) {
					actorToFollowId = ordersActorFollow[i].dataGet<EntityId>("ActorToFollowId");
					break;
				}
			}
		}
	}

	sf::Vector2f actorPosition;

	if (actorToFollowId != UINT32_MAX) {
		actorPosition = EntityManager::entityGet(actorToFollowId).entityComponentGet<ComponentPosition>()->position;
	}
	else return;

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = actorPosition;
	eventGoTo->desiredDist = 64.f;

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = actorPosition;
}

UtilityStateScore StatePointGoTo::condition(const ActorDataHolder& actorData, const ActorBlackboard& actorBlackboard) {
	uint16_t score = 0;

	if (actorBlackboard.dataHas("OrdersToSelf")) {
		OrderVector orders = actorBlackboard.dataGet<OrderVector>("OrdersToSelf");
		OrderSubVector& ordersPointGoTo = orders[uint16_t(OrderType::PointGoTo)];

		if (ordersPointGoTo.size() <= 0) goto noOrdersLabel;

		if (actorBlackboard.dataHas("LeaderId")) {
			for (uint16_t i = 0; i < ordersPointGoTo.size(); i++) {
				if (ordersPointGoTo[i].dataGet<EntityId>("OrderFromId") == actorBlackboard.dataGet<EntityId>("LeaderId")) {
					score = 75;
					break;
				}
			}
		}
	}
	// used when there are no orders to follow
	noOrdersLabel:

	return score;
}
void StatePointGoTo::behavior(Entity& actor, const ActorBlackboard& actorBlackboard) {

	sf::Vector2f pointToGoTo;

	if (actorBlackboard.dataHas("OrdersToSelf")) {
		OrderVector orders = actorBlackboard.dataGet<OrderVector>("OrdersToSelf");
		OrderSubVector& ordersPointGoTo = orders[uint16_t(OrderType::PointGoTo)];

		if (actorBlackboard.dataHas("LeaderId")) {
			for (uint16_t i = 0; i < ordersPointGoTo.size(); i++) {
				if (ordersPointGoTo[i].dataGet<EntityId>("OrderFromId") == actorBlackboard.dataGet<EntityId>("LeaderId")) {
					pointToGoTo = ordersPointGoTo[i].dataGet<sf::Vector2f>("PointToGoTo");
					break;
				}
			}
		}
	}

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = pointToGoTo;
	eventGoTo->desiredDist = 64.f;

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = pointToGoTo;
}



