#ifndef __ORDER_HANDLER_H__
#define __ORDER_HANDLER_H__

#include "OrderTypes.hpp"
#include <ECS/TypeDefinitions.hpp>
#include "../Blackboard/ActorBlackboard.hpp"
#include "../Data/ActorDataHolder.hpp"

namespace OrderHandler {
	bool actorOrderShouldAccept(const EntityId actorId, const ActorBlackboard& actorBlackboard, const OrderData& orderData);
	// checks whether an order is from a certain actor and if it's to a certain actor
	bool actorOrderIsFromActorToActor(const EntityId actorIdFrom, const EntityId actorIdTo, const OrderData& orderData);
	// checks whether an order is from any actor in a set of actors and if it's to a certain actor
	bool actorOrderIsFromActorSetToActor(const std::set<EntityId> actorIdsFrom, const EntityId actorIdTo, const OrderData& orderData);
};

#endif