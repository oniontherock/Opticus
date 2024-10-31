#include "ObjectVision.hpp"




ObjectVision::ObjectVision() {
	objectsSeenCreate();
}
ObjectVision::ObjectVision(sf::Vector2f _castPosition) :
	ObjectVision()
{
	castPosition = _castPosition;
}

void ObjectVision::objectsSeenCreate() {
	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {
		objectsSeenVector.push_back(std::vector<EntityId>());
	}
}
void ObjectVision::objectsSeenClear() {
	for (uint16_t i = 0; i < static_cast<uint16_t>(ObjectType::SIZE); i++) {
		objectsSeenVector.at(i).clear();
	}
}

void ObjectVision::update(float fromX, float fromY, float angleTo, float coneSize, float rayMaxDist, uint32_t rayCount) {

	castPosition.position = sf::Vector2f(fromX, fromY);

	// cast the rays, updating the objectsSeenVector
	raysCast(angleTo, coneSize, rayMaxDist, rayCount);
}

ObjectIdVector& ObjectVision::objectsSeenGet() {
	return objectsSeenVector;
}

void ObjectVision::raysCast(float angleTo, float coneSize, float rayMaxDist, uint32_t rayCount) {

	objectsSeenClear();

	GameLevel* gameLevel = GameLevelGrid::levelGet(castPosition.level);

	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / rayCount;

	if (castPosition.position.x < 0 || castPosition.position.x >= gameLevel->levelSize.x || castPosition.position.y < 0 || castPosition.position.y >= gameLevel->levelSize.y) {
		return;
	}

	auto& objectGrid = gameLevel->objectGrid;

	auto& distortionGrid = gameLevel->distortionGrid;

	constexpr double posMultiplier = 1.0 / 255.0;

	for (uint32_t curRayInd = 0; curRayInd < rayCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = angleTo + (float(curRayInd) * rayAngleDifference);

		sf::Vector2f rayPosition = castPosition.position;
		sf::Vector2f rayHeading = sf::Vector2f(cos(rayRotation), sin(rayRotation));

		sf::Vector2u rayPositionCell = objectGrid.coordinatesWorldToCell(rayPosition.x, rayPosition.y);
		sf::Vector2u rayPositionCellPrev;

		// the assumed dist that the ray has moved.
		// note the "assumed", because the ray may have moved more or less, due to distortions.
		float curDist = 0.f;
		while (curDist < rayMaxDist) {
			curDist += 1.f;

			auto& distortion = distortionGrid.cellGet(
				uint16_t(rayPosition.x * distortionGrid.distortionCellMultiplierX),
				uint16_t(rayPosition.y * distortionGrid.distortionCellMultiplierY)
			);

			// check if there are any distortions at the ray's position
			if (distortion.distortions.size() > 0) {
				// apply the distortion at the rayPosition to the ray.
				distortion.headingApplyDistortion(rayHeading, rayPosition);

				if (Vector2fMath::lengthSqrd(rayHeading) <= 0.001f * 0.001f) break;
			}

			// move the rayPosition by the rayHeading.
			// keep in mind that a distortion was just applied to the heading, though the distortion may not have done anything.
			rayPosition += rayHeading;
			// make sure the rayPosition is in the bounds of the level.
			if (rayPosition.x < 0 || rayPosition.x >= gameLevel->levelSize.x || rayPosition.y < 0 || rayPosition.y >= gameLevel->levelSize.y) break;

			rayPositionCell = objectGrid.coordinatesWorldToCell(rayPosition.x, rayPosition.y);

			// check if we have changed cells before getting ids, for the sake of performance
			if (rayPositionCell != rayPositionCellPrev) {

				rayPositionCellPrev = rayPositionCell;

				// set of EntityIds in this cell
				std::set<EntityId> cellIdsSet = objectGrid.cellIdsGet(rayPositionCell);

				std::vector<EntityId> cellIdsVector(cellIdsSet.begin(), cellIdsSet.end());

				for (uint16_t i = 0; i < cellIdsVector.size(); i++) {

					uint16_t objectType = static_cast<uint16_t>(ObjectRegistry::entityObjectTypeGet(cellIdsVector[i]));

					// only add the object to the objectsSeenVector if it's not already in it
					if (std::count(objectsSeenVector[objectType].begin(), objectsSeenVector[objectType].end(), cellIdsVector[i]) <= 0) {
						objectsSeenVector[objectType].push_back(cellIdsVector[i]);
					}
				}
			}
		}
	}
}






