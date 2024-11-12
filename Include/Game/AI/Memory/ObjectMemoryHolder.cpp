#include "ObjectMemoryHolder.hpp"

ObjectMemoryHolder::ObjectMemoryHolder() {
	objectMemoriesCreate();
}


void ObjectMemoryHolder::memoryUpdate(std::vector<std::vector<EntityId>> objectsToMemorize) {

	uint32_t timeNow = TimeHandler::timeSimulatedGet();
	float delta = float(timeNow - updateTimeLast) / 1000.f;
	updateTimeLast = timeNow;

	// iterate over all object types
	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {

		// get the current sub-vector of the objectMemoriesVector
		auto& memoryVector = objectMemoriesVector.at(i);
		// get the current sub-vector of the objectsToMemorize
		auto& toMemorizeVector = objectsToMemorize.at(i);

		// iterate over each ObjectMemory in the sub-vector
		for (uint16_t j = 0; j < memoryVector.size(); j++) {

			auto itr = std::find(toMemorizeVector.begin(), toMemorizeVector.end(), memoryVector[j].first);

			// if the objectsToMemorize contains the current ObjectMemory, that means we're seeing an object we've previously memorized,
			// therefore, set the memory timer to 0
			if (itr != toMemorizeVector.end()) {
				memoryVector[j].second.reset();
				// erase the object from the objectsToMemorize, as resetting the memory time to 0 effectively memorizes it
				toMemorizeVector.erase(itr);
			}
			else {
				if (memoryVector[j].second.updateAutoReset(delta)) {
					memoryVector.erase(memoryVector.begin() + j);
				}
			}
		}

		// iterate through the remaining objects to memorize and add them to the memoryVector,
		// since we've previously checked if any of the objects are already present in the vector, we can avoid unnecessary checks here.
		for (uint16_t j = 0; j < toMemorizeVector.size(); j++) {
			memoryVector.push_back(ObjectMemory(toMemorizeVector[j], Cooldown(memoryMaxLife)));
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


