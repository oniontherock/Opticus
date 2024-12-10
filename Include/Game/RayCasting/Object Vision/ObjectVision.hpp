#ifndef __OBJECT_VISION_H__
#define __OBJECT_VISION_H__

#include "../../World/Objects/ObjectRegistry.hpp"
#include "../../World/Objects/ObjectTypes.hpp"
#include "../ACECS/GameLevel.hpp"
#include "../RayCaster.hpp"
#include <cstdint>
#include <ECS/TypeDefinitions.hpp>
#include <set>
#include <vector>

// vector of vectors of EntityIds,
// entities are placed into the sub-vectors based off of their ObjectType.
// so if an entity is an object of type Door, which is, for example, at position 4 in the ObjectType enum, the entity's Id will be placed in sub-vector 4.
typedef std::vector<std::vector<EntityId>> ObjectIdVector;

struct ObjectVision : public RayCaster {

	ObjectVision();
	ObjectVision(sf::Vector2f _castPosition);

	void update(float fromX, float fromY, float angleTo, float coneSize, float rayMaxDist, uint32_t rayCount) override;

	ObjectIdVector& objectsSeenGet();

	friend std::ofstream& operator<<(std::ofstream& str, ObjectVision& item);
	friend std::ifstream& operator>>(std::ifstream& str, ObjectVision& item);

protected:

	// set of EntityIdObjectTypePairs
	ObjectIdVector objectsSeenVector;

	void objectsSeenCreate();
	void objectsSeenClear();

	void raysCast(float coneSize, float rayMaxDist, uint32_t rayCount) override;
};

#endif