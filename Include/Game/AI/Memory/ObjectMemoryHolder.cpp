#include "ObjectMemoryHolder.hpp"

void ObjectMemoryHolder::memoryUpdate(std::vector<std::vector<EntityId>> objectsToMemorize) {

	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {
		for (uint16_t j = 0; j < objectsToMemorize.at(i).size(); j++) {

			auto& memoryVector = objectMemoriesVector.at(i);

			EntityId idToMemorize = objectsToMemorize.at(i)[j];

			// idToMemorize is not present
			if (!memoryHas(idToMemorize, i)) {
				memoryVector.push_back(ObjectMemory(idToMemorize, Cooldown(memoryMaxLife)));
			}
			// idToMemorize is present
			else {
				// reset the timer on the memory to 0
				memoryGetFromId(idToMemorize, i).second.reset();
			}
		}
	}
}
void ObjectMemoryHolder::objectMemoriesCreate() {
	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {
		objectMemoriesVector.push_back(std::vector<ObjectMemory>());
	}
}
void ObjectMemoryHolder::objectMemoriesClear() {
	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {
		objectMemoriesVector.at(i).clear();
	}
}

std::vector<EntityId> ObjectMemoryHolder::objectIdsGetOfType(uint16_t objectType) {
	const std::vector<ObjectMemory>& memoriesVector = memoriesGetOfType(objectType);

	std::vector<EntityId> entityIdsVector;

	for (uint16_t i = 0; i < memoriesVector.size(); i++) {
		entityIdsVector.push_back(memoriesVector[i].first);
	}

	return entityIdsVector;
}
std::vector<EntityId> ObjectMemoryHolder::objectIdsGetOfType(ObjectType objectType) {
	return objectIdsGetOfType(static_cast<uint16_t>(objectType));
}

std::vector<ObjectMemory>& ObjectMemoryHolder::memoriesGetOfType(uint16_t objectType) {
	return objectMemoriesVector.at(objectType);
}
std::vector<ObjectMemory>& ObjectMemoryHolder::memoriesGetOfType(ObjectType objectType) {
	return memoriesGetOfType(static_cast<uint16_t>(objectType));
}


bool ObjectMemoryHolder::memoryHas(EntityId entityId, uint16_t objectType) {

	std::vector<ObjectMemory>& memoryVector = memoriesGetOfType(objectType);

	std::vector<ObjectMemory>::iterator itr = std::find_if(memoryVector.begin(), memoryVector.end(), [&entityId](ObjectMemory& memory) {
		return memory.first == entityId;
		});

	return itr != memoryVector.end();
}
bool ObjectMemoryHolder::memoryHas(EntityId entityId, ObjectType objectType) {
	return memoryHas(entityId, static_cast<uint16_t>(objectType));
}
bool ObjectMemoryHolder::memoryHas(EntityId entityId) {
	return memoryHas(static_cast<uint16_t>(ObjectRegistry::entityObjectTypeGet(entityId)));
}

bool ObjectMemoryHolder::memoryHasType(uint16_t objectType) {
	return objectMemoriesVector.at(objectType).size() > 0;
}
bool ObjectMemoryHolder::memoryHasType(ObjectType objectType) {
	return memoryHasType(static_cast<uint16_t>(objectType));
}

ObjectMemory& ObjectMemoryHolder::memoryGetFromId(EntityId entityId, uint16_t objectType) {

	std::vector<ObjectMemory>& memoryVector = memoriesGetOfType(objectType);

	std::vector<ObjectMemory>::iterator itr = std::find_if(memoryVector.begin(), memoryVector.end(), [&entityId](ObjectMemory& memory) {
		return memory.first == entityId;
		});

	return *itr;
}
ObjectMemory& ObjectMemoryHolder::memoryGetFromId(EntityId entityId, ObjectType objectType) {
	return memoryGetFromId(entityId, static_cast<uint16_t>(objectType));
}
ObjectMemory& ObjectMemoryHolder::memoryGetFromId(EntityId entityId) {
	return memoryGetFromId(entityId, static_cast<uint16_t>(ObjectRegistry::entityObjectTypeGet(entityId)));
}


