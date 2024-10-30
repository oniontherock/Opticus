#ifndef __OBJECT_CELL_H__
#define __OBJECT_CELL_H__

#include <ECS/TypeDefinitions.hpp>
#include <vector>
#include <set>
#include "../../../../Common/Cooldown.hpp"
#include "../../../../Common/TimeHandler.hpp"

// an object that contains a list of EntityIds.
// for moving objects, they must remove their previous position from the object grid,
// move, and add their new position.
struct ObjectCell {

	// adds an EntityId to the idsVector
	void idAdd(EntityId id);
	// removes an EntityId from the idsVector
	void idRemove(EntityId id);
	// check if the cell contains the id
	bool idHas(EntityId id);
	// returns a const reference to the idsVector
	const std::set<EntityId>& idsGet();
private:
	// vector of EntityIds,
	std::set<EntityId> idsVector;
};

#endif