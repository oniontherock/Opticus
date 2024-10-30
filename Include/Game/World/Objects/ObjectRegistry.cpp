#include "ObjectRegistry.hpp"


std::vector<ObjectTypes> ObjectRegistry::objectsVector = std::vector<ObjectTypes>(MAX_ENTITIES);

void ObjectRegistry::entityObjectTypeAssign(EntityId id, ObjectTypes objectType) {
	objectsVector[id] = objectType;
}
void ObjectRegistry::entityObjectTypeRemove(EntityId id) {
	entityObjectTypeAssign(id, ObjectTypes::Null);
}
const ObjectTypes ObjectRegistry::entityObjectTypeGet(EntityId id) {
	return objectsVector[id];
}
