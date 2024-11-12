#include "OrderHandler.hpp"


bool OrderHandler::actorOrderShouldAccept(const EntityId actorId, const ActorBlackboard& actorBlackboard, const OrderData& orderData) {
	
	// is the order from the actor's leader?
	bool isFromLeader = false;
	// is the order to the actor?
	bool isToActor = false;

	// check if order is from leader
	if (actorBlackboard.dataHas("LeaderId")) {
		if (orderData.dataGet<EntityId>("OrderFromId") == actorBlackboard.dataGet<EntityId>("LeaderId")) {
			isFromLeader = true;
		}
	}

	if (orderData.dataGet<OrderToSet>("OrderToIds").contains(actorId)) {
		isToActor = true;
	}

	return isFromLeader && isToActor;
}

bool OrderHandler::actorOrderIsFromActorToActor(const EntityId actorIdFrom, const EntityId actorIdTo, const OrderData& orderData) {
	// is the order from the actorIdFrom?
	bool isFromActor = false;
	// is the order to the actorIdTo?
	bool isToActor = false;

	// check if order is from the actorIdFrom
	if (orderData.dataGet<EntityId>("OrderFromId") == actorIdFrom) {
		isFromActor = true;
	}

	if (orderData.dataGet<OrderToSet>("OrderToIds").contains(actorIdTo)) {
		isToActor = true;
	}

	return isFromActor && isToActor;
}

bool OrderHandler::actorOrderIsFromActorSetToActor(const std::set<EntityId> actorIdsFrom, const EntityId actorIdTo, const OrderData& orderData) {
	// is the order from the actorIdFrom?
	bool isFromActor = false;
	// is the order to the actorIdTo?
	bool isToActor = false;

	// check if order is from the actorIdFrom
	if (actorIdsFrom.contains(orderData.dataGet<EntityId>("OrderFromId"))) {
		isFromActor = true;
	}

	if (orderData.dataGet<OrderToSet>("OrderToIds").contains(actorIdTo)) {
		isToActor = true;
	}

	return isFromActor && isToActor;
}

