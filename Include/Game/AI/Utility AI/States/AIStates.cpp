#include "AIStates.hpp"
#include "../../../../Common/NumberGenerator.hpp"
#include "../../Actors/Orders/OrderHandler.hpp"
#include "../../../ACECS/ECSRegistry.hpp"

using namespace UtilityStates;
using namespace EntityComponents;
using namespace EntityEvents;

UtilityStateScore StateIdle::condition(const Entity&, const ActorDataHolder&, const ActorBlackboard&) {
	UtilityStateScore score = 10;

	return score;
}
void StateIdle::behavior(Entity& actor, ActorBlackboard& actorBlackboard) {
	
	if (!actorBlackboard.dataHas("PointWander")) return;

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = actorBlackboard.dataGet<sf::Vector2f>("PointWander");
	eventGoTo->desiredDist = 4.f;

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = actorBlackboard.dataGet<sf::Vector2f>("PointWander");
}

UtilityStateScore StateActorFollow::condition(const Entity& actor, const ActorDataHolder&, const ActorBlackboard& actorBlackboard) {
	UtilityStateScore score = 0;

	if (actorBlackboard.dataHas("Orders") && actorBlackboard.dataHas("LeaderId")) {

		// get all orders
		OrderVector* orders = actorBlackboard.dataGet<OrderVector*>("Orders");
		// get all of type ActorFollow
		OrderSubVector& ordersActorFollow = orders->at(uint16_t(OrderType::ActorFollow));

		for (uint16_t i = 0; i < ordersActorFollow.size(); i++) {

			// get current order of type ActorFollow
			OrderData& orderData = ordersActorFollow[i];

			// skip if the order is not to us or is not from our leader
			if (!OrderHandler::actorOrderIsFromActorToActor(actorBlackboard.dataGet<EntityId>("LeaderId"), actor.Id, orderData)) continue;

			score = 75;
		}
	}

	return score;
}
void StateActorFollow::behavior(Entity& actor, ActorBlackboard& actorBlackboard) {

	sf::Vector2f actorToFollowPosition;
	EntityId actorToFollowId = 99999;
	float distSqrdToTarget = 999999;

	if (actorBlackboard.dataHas("Orders") && actorBlackboard.dataHas("LeaderId")) {

		// get all orders
		OrderVector* orders = actorBlackboard.dataGet<OrderVector*>("Orders");
		// get all of type ActorFollow
		OrderSubVector& ordersActorFollow = orders->at(uint16_t(OrderType::ActorFollow));

		for (uint16_t i = 0; i < ordersActorFollow.size(); i++) {
			
			// get current order of type ActorFollow
			OrderData& orderData = ordersActorFollow[i];

			// skip if the order is not to us or is not from our leader
			if (!OrderHandler::actorOrderIsFromActorToActor(actorBlackboard.dataGet<EntityId>("LeaderId"), actor.Id, orderData)) continue;

			// get the id actor we've been told to follow
			actorToFollowId = orderData.dataGet<EntityId>("ActorToFollowId");
			// get the instance of the actor we've been told to follow
			Entity& actorToFollow = EntityManager::entityGet(actorToFollowId);

			// get the blackboard of the actor we've been told to follow
			ActorBlackboard& actorToFollowBlackboard = actorToFollow.entityComponentGet<ComponentActorBlackboard>()->actorBlackboard;

			// if the actor we've been told to follow has no followers, great! break the loop and follow them
			if (!actorToFollowBlackboard.dataHas("Followers")) break;

			// if they do have followers, get the followers
			std::vector<EntityId> actorToFollowFollowersVector = actorToFollowBlackboard.dataGet<std::vector<EntityId>>("Followers");

			// follow target if we are closest and ignore following target's followers,
			// note that the Followers vector is sorted by distance
			if (actorToFollowFollowersVector[0] == actor.Id) break;

			for (uint16_t j = 0; j < actorToFollowFollowersVector.size(); j++) {
				if (actorToFollowFollowersVector[i] == actor.Id) continue;

				// follow the first follower of the actor we've been told to follow
				actorToFollowId = actorToFollowFollowersVector[j];
				// set the ActorToFollowId in the orderData to the new actor we are following, effectively changing the actor we've been told to follow
				orderData.dataSet("ActorToFollowId", actorToFollowId);

				// break if we are not in the actorToFollowFollowersVector
				if (std::find(actorToFollowFollowersVector.begin(), actorToFollowFollowersVector.end(), actor.Id) == actorToFollowFollowersVector.end()) break;

				// if we are in the actorToFollowFollowersVector, remove ourselves, because we are now following someone else
				actorToFollowFollowersVector.erase(std::find(actorToFollowFollowersVector.begin(), actorToFollowFollowersVector.end(), actor.Id));
				actorToFollowBlackboard.dataSet<std::vector<EntityId>>("Followers", actorToFollowFollowersVector);

				break;
			}
		}
	}

	Entity& actorToFollow = EntityManager::entityGet(actorToFollowId);
	auto* actorToFollowEventFollowed = actorToFollow.entityEventAddAndGet<EventFollowed>();

	distSqrdToTarget = Vector2fMath::distSqrd(actorToFollow.entityComponentGet<ComponentPosition>()->position, actor.entityComponentGet<ComponentPosition>()->position);

	actorToFollowEventFollowed->followerId = actor.Id;
	actorToFollowEventFollowed->distSqrd = distSqrdToTarget;


	actorToFollowPosition = EntityManager::entityGet(actorToFollowId).entityComponentGet<ComponentPosition>()->position;

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = actorToFollowPosition;
	eventGoTo->desiredDist = 64.f;

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = actorToFollowPosition;
}

UtilityStateScore StateActorFollowTactical::condition(const Entity& actor, const ActorDataHolder&, const ActorBlackboard& actorBlackboard) {
	UtilityStateScore score = 0;

	if (actorBlackboard.dataHas("Orders") && actorBlackboard.dataHas("LeaderId")) {

		// get all orders
		OrderVector* orders = actorBlackboard.dataGet<OrderVector*>("Orders");
		// get all of type ActorFollow
		OrderSubVector& ordersActorFollow = orders->at(uint16_t(OrderType::ActorFollow));

		for (uint16_t i = 0; i < ordersActorFollow.size(); i++) {

			// get current order of type ActorFollow
			OrderData& orderData = ordersActorFollow[i];

			// skip if order is not to us
			if (!orderData.dataGet<OrderToSet>("OrderToIds").contains(actor.Id)) continue;

			// skip if order is not from leader
			if (orderData.dataGet<EntityId>("OrderFromId") != actorBlackboard.dataGet<EntityId>("LeaderId")) continue;

			score = 75;
		}
	}

	return score;
}
void StateActorFollowTactical::behavior(Entity& actor, ActorBlackboard& actorBlackboard) {

	sf::Vector2f actorToFollowPosition;

	if (actorBlackboard.dataHas("OrdersToSelf")) {
		OrderVector orders = actorBlackboard.dataGet<OrderVector>("OrdersToSelf");
		OrderSubVector& ordersActorFollow = orders[uint16_t(OrderType::ActorFollow)];

		if (actorBlackboard.dataHas("LeaderId")) {
			for (uint16_t i = 0; i < ordersActorFollow.size(); i++) {
				if (ordersActorFollow[i].dataGet<EntityId>("OrderFromId") == actorBlackboard.dataGet<EntityId>("LeaderId")) {

					auto objectMemory = actorBlackboard.dataGet<ObjectMemoryHolder>("Memory");

					ObjectMemory& actorMemory = objectMemory.memoryGetFromId(ordersActorFollow[i].dataGet<EntityId>("OrderFromId"));

					if (actorMemory.second.value < 10.f) {
						actorToFollowPosition = EntityManager::entityGet(actorMemory.first).entityComponentGet<ComponentPosition>()->position;
					}
					break;
				}
			}
		}
	}

	if (actorToFollowPosition == sf::Vector2f(0, 0)) {
		return;
	}

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = actorToFollowPosition;
	eventGoTo->desiredDist = 64.f;

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = actorToFollowPosition;
}
UtilityStateScore StatePointGoTo::condition(const Entity& actor, const ActorDataHolder&, const ActorBlackboard& actorBlackboard) {
	UtilityStateScore score = 0;

	if (actorBlackboard.dataHas("Orders") && actorBlackboard.dataHas("LeaderId")) {

		// get all orders
		OrderVector* orders = actorBlackboard.dataGet<OrderVector*>("Orders");
		// get all of type ActorFollow
		OrderSubVector& ordersPointGoTo = orders->at(uint16_t(OrderType::PointGoTo));

		for (uint16_t i = 0; i < ordersPointGoTo.size(); i++) {

			// get current order of type ActorFollow
			OrderData& orderData = ordersPointGoTo[i];

			// skip if order is not to us
			if (!orderData.dataGet<OrderToSet>("OrderToIds").contains(actor.Id)) continue;

			// skip if order is not from leader
			if (orderData.dataGet<EntityId>("OrderFromId") != actorBlackboard.dataGet<EntityId>("LeaderId")) continue;

			score = 75;
		}
	}

	return score;
}
void StatePointGoTo::behavior(Entity& actor, ActorBlackboard& actorBlackboard) {

	sf::Vector2f pointToGoTo;

	if (actorBlackboard.dataHas("Orders") && actorBlackboard.dataHas("LeaderId")) {

		// get all orders
		OrderVector* orders = actorBlackboard.dataGet<OrderVector*>("Orders");
		// get all of type ActorFollow
		OrderSubVector& ordersPointGoTo = orders->at(uint16_t(OrderType::PointGoTo));

		for (uint16_t i = 0; i < ordersPointGoTo.size(); i++) {

			// get current order of type ActorFollow
			OrderData& orderData = ordersPointGoTo[i];

			// skip if order is not to us
			if (!orderData.dataGet<OrderToSet>("OrderToIds").contains(actor.Id)) continue;

			// skip if order is not from leader
			if (orderData.dataGet<EntityId>("OrderFromId") != actorBlackboard.dataGet<EntityId>("LeaderId")) continue;

			pointToGoTo = orderData.dataGet<sf::Vector2f>("PointToGoTo");
		}
	}

	auto* eventGoTo = actor.entityEventAddAndGet<EventActorGoTo>();
	eventGoTo->positionTo = pointToGoTo;
	eventGoTo->desiredDist = 16.f;

	auto* eventTurnTo = actor.entityEventAddAndGet<EventActorTurnTo>();
	eventTurnTo->positionTo = pointToGoTo;
}



