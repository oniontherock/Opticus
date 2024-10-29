#include "ObjectRegistry.hpp"


std::vector<ObjectTypes> ObjectRegistry::objects = std::vector<ObjectTypes>(MAX_ENTITIES);

void ObjectRegistry::entityObjectTypeAssign(EntityId id, ObjectTypes objectType) {
	objects[id] = objectType;
}

const ObjectTypes ObjectRegistry::entityObjectTypeGet(EntityId id) {
	return objects[id];
}
