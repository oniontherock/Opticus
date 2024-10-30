#include "ObjectCell.hpp"
#include <cstdint>


void ObjectCell::idAdd(EntityId id) {
	idsVector.insert(id);
}
void ObjectCell::idRemove(EntityId id) {
	idsVector.erase(id);
}
bool ObjectCell::idHas(EntityId id) {
	return idsVector.count(id);
}
const std::set<EntityId>& ObjectCell::idsGet() {
	return idsVector;
}

