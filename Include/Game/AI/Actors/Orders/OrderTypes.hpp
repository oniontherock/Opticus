#ifndef __ORDER_TYPES_H__
#define __ORDER_TYPES_H__

#include <cstdint>
#include <set>
#include <ECS/TypeDefinitions.hpp>
#include "../../../../Common/DataCache.hpp"

enum class OrderType : uint16_t {
	ActorFollow, // tells the actor to follow another actor.
	ActorFollowTactical, // tells the actor to follow another actor in a tactical way (I.E. in a swat formation kind of line).
	PointGoTo, // tells the actor to go to the point.
	OrdersDismiss, // tells the actor that any orders given by the actor giving this order may be dismissed.
	OrdersEnforce, // tells the actor that any orders given by the actor giving this order are to be placed at a higher priority.
	SIZE, // size of OrderType enum.
};

// DataCache for orders, stores the OrderType, the EntityId of the entity who gave the order, who the order is to (is a set), and other type-specific data.
typedef DataCache OrderData;
// set of EntityIds, the ids are who the order is to.
typedef std::set<EntityId> OrderToSet;
// vector of orders of a specific type
typedef std::vector<OrderData> OrderSubVector;
// vector of vectors of OrderData, each sub-vector is filled with orders of a specific type, so all ActorFollow orders are in one sub-vector
typedef std::vector<OrderSubVector> OrderVector;

#endif