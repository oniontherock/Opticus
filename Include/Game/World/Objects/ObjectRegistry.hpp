#ifndef __OBJECT_REGISTRY_H__
#define __OBJECT_REGISTRY_H__

#include "ObjectTypes.hpp"
#include <ECS/TypeDefinitions.hpp>
#include <vector>

// 
struct ObjectRegistry {

	static void entityObjectTypeAssign(EntityId id, ObjectTypes objectType);
	static const ObjectTypes entityObjectTypeGet(EntityId id);

private:
	// list of ObjectTypes accessed by EntityIds.
	// each entity can have only one object type, not every entity must have an object type.
	// is of size MAX_ENTITIES
	static std::vector<ObjectTypes> objects;


};


#endif