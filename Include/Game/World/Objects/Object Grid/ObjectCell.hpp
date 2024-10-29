#ifndef __OBJECT_CELL_H__
#define __OBJECT_CELL_H__

#include <ECS/TypeDefinitions.hpp>
#include <vector>
#include "../../../../Common/Cooldown.hpp"
#include "../../../../Common/TimeHandler.hpp"

// duration how long an EntityId should exist before being removed from an ObjectCell
typedef Cooldown IdDuration;
// pair of an EntityId and an IdDuration
typedef std::pair<EntityId, IdDuration> IdDurationPair;

// an object that contains a list of EntityIds and durations for them.
// basically, some object (usually entities) add an Id to the IdsVector with a duration,
// the duration is the amount of time that the ObjectCell will hold the Id for,
// once the duration runs out for that Id, the Id is removed from the IdsVector.
//
// it's done this way so that, for example, if we have a moving object, like a bullet,
// it doesn't have to remove it's Id from the ObjectCell it was previously on, move, then add it's Id to the new ObjectCell, and repeat.
// instead, it can add it's Id to the ObjectGrid at it's location for just one frame, and it never has to remove it's Id from the ObjectGrid,
// because it's Id automatically gets removed after one frame.
struct ObjectCell {

	ObjectCell();

	// adds a IdDurationPair to the IdsVector
	void idAdd(IdDurationPair idDurationPair);
	// creates an IdDurationPair from the id and the duration and adds it to the IdsVector
	void idAdd(EntityId id, IdDuration duration);
	// creates an IdDurationPair from the id and the duration and adds it to the IdsVector
	void idAdd(EntityId id, float duration);
	// returns a list of 
	const std::vector<EntityId> idsGet();
private:
	// vector of EntityIds and their durations,
	// when a duration runs out, the IdDurationPair is removed from the vector
	std::vector<IdDurationPair> IdsVector;

	uint32_t updateTimeLast;
	// updates the IdDurations in the IdsVector
	void idsVectorUpdate();
};

#endif