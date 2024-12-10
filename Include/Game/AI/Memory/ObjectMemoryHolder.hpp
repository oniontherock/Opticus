#ifndef __OBJECT_MEMORY_HOLDER_H__
#define __OBJECT_MEMORY_HOLDER_H__

#include <Auxiliary/Cooldown.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include "../../World/Objects/ObjectTypes.hpp"
#include "../../World/Objects/ObjectRegistry.hpp"
#include <ECS/TypeDefinitions.hpp>
#include <utility>
#include <vector>

// "memory" of an object,
// the EntityId is the id of the object, and the Cooldown is how long since the object was last seen.
typedef std::pair<EntityId, Cooldown> ObjectMemory;

typedef std::vector<std::vector<ObjectMemory>> ObjectMemoryVector;

struct ObjectMemoryHolder {

	ObjectMemoryHolder();

	ObjectMemoryVector objectMemoriesVector;

	void memoryUpdate(std::vector<std::vector<EntityId>> objectsToMemorize);
	//void memoryUpdate(std::vector<std::vector<EntityId>>* objectsToMemorize);

	// gets every EntityId of the specified ObjectType
	std::vector<EntityId> objectIdsGetOfType(uint16_t objectType);
	// gets every EntityId of the specified ObjectType
	std::vector<EntityId> objectIdsGetOfType(ObjectType objectType);
	// gets every ObjectMemory of the specified ObjectType
	std::vector<ObjectMemory>& memoriesGetOfType(uint16_t objectType);
	// gets every ObjectMemory of the specified ObjectType
	std::vector<ObjectMemory>& memoriesGetOfType(ObjectType objectType);

	// checks whether the objectMemoriesVector contains the entityId.
	bool memoryHas(EntityId entityId, uint16_t objectType);
	// checks whether the objectMemoriesVector contains the entityId.
	bool memoryHas(EntityId entityId, ObjectType objectType);
	// checks whether the objectMemoriesVector contains the entityId.
	bool memoryHas(EntityId entityId);

	// checks if the memory contains any object of the specified type.
	bool memoryHasType(uint16_t objectType);
	// checks if the memory contains any object of the specified type.
	bool memoryHasType(ObjectType objectType);

	// gets the ObjectMemory corresponding to the entityId,
	// error if the memory does not exist.
	ObjectMemory& memoryGetFromId(EntityId entityId, uint16_t objectType);
	// gets the ObjectMemory corresponding to the entityId,
	// error if the memory does not exist.
	ObjectMemory& memoryGetFromId(EntityId entityId, ObjectType objectType);
	// gets the ObjectMemory corresponding to the entityId,
	// error if the memory does not exist.
	ObjectMemory& memoryGetFromId(EntityId entityId);

	friend std::ofstream& operator<<(std::ofstream& str, ObjectMemoryHolder& item);
	friend std::ifstream& operator>>(std::ifstream& str, ObjectMemoryHolder& item);

private:
	void objectMemoriesCreate();
	void objectMemoriesClear();

	// the time of the last update in milliseconds,
	// based off of simulated time
	uint32_t updateTimeLast;

	// how long until a memory is deleted
	float memoryMaxLife = 120.f;
};


#endif
