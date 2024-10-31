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

// pair of an EntityId and an ObjectType
typedef std::pair<EntityId, ObjectType> EntityIdObjectTypePair;

struct ObjectVision : public RayCaster {

	ObjectVision();
	ObjectVision(sf::Vector2f _castPosition);

	void update(float fromX, float fromY, float angleTo, float coneSize, uint32_t rayCount) override;

	const std::set<EntityIdObjectTypePair>& objectsSeenGet();

protected:

	// set of EntityIdObjectTypePairs
	std::set<EntityIdObjectTypePair> objectsSeenSet;

	void raysCast(float angleTo, float coneSize, uint32_t rayCount) override;
};

#endif