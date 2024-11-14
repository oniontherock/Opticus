#include "AStarCell.hpp"
#include "AStarGrid.hpp"
#include <World/LevelGrid.hpp>
#include "../../../ACECS/GameLevel.hpp"
#include <iostream>

AStarCell::AStarCell() :
	cellPositionGrid(sf::Vector2u(0, 0)),
	cellPositionWorld(sf::Vector2f(0, 0))
{
	resetPathingData();
}

AStarCell::AStarCell(AStarCellPosition _cellPositionGrid, sf::Vector2f aStarGridCellSize) :
	cellPositionGrid(_cellPositionGrid),
	cellPositionWorld(sf::Vector2f(cellPositionGrid.x * aStarGridCellSize.x, cellPositionGrid.y * aStarGridCellSize.y))
{
	resetPathingData();
}

bool AStarCell::operator< (const AStarCell& other) const {
	return costF < other.costF;
}
bool AStarCell::operator== (const AStarCell& other) const {
	return cellPositionGrid == other.cellPositionGrid;
}



void AStarCell::costHCalculate() {
	costH = 0;
}
void AStarCell::resetCosts() {
	costG = 0;
	costH = 0;
	costF = 0;
}
void AStarCell::resetPathingData() {
	resetCosts();

	cellParentFromAxis.x, cellParentFromAxis.y = 1;
	// set to 4 because element 4 is the the cell itself
	cellParentNeighborIndex = 8;
}



void AStarCell::neighborCellsVectorUpdate(AStarGrid& aStarGrid, sf::Vector2f aStarGridCellSize, WorldDistortionGrid& distortionGrid) {

	// clear neighborCellsVector
	neighborCellsVector.clear();

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);


	// iterate around neighbors and cast rays to find who is actually our neighbors.
	for (int16_t x = -1; x <= 1; x++) {
		for (int16_t y = -1; y <= 1; y++) {
			
			if (x == 0 && y == 0) {
				neighborCellsVector.push_back(cellPositionGrid);
				neighborCellsValidVector.push_back(false);
				continue;
			}

			const float rayRotation = float(atan2(y, x));

			sf::Vector2f rayPosition = cellPositionWorld;
			sf::Vector2f rayHeading = sf::Vector2f(cos(rayRotation), sin(rayRotation));


			// the assumed dist that the ray has moved.
			// note the "assumed", because the ray may have moved more or less, due to distortions.
			float curDist = 0.f;
			while (curDist * curDist < Vector2fMath::lengthSqrd(x * aStarGridCellSize.x, y * aStarGridCellSize.y)) {
				curDist += 1.f;

				if (rayPosition.x < 0 || rayPosition.x >= gameLevel->levelSize.x || rayPosition.y < 0 || rayPosition.y >= gameLevel->levelSize.y) {
					break;
				}

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
			}

			if (!aStarGrid.worldPosIsInGridFull(rayPosition)) {
				neighborCellsVector.push_back(aStarGrid.coordinatesWorldToCell(rayPosition));
				neighborCellsValidVector.push_back(false);
			}
			else {
				// get the neighbor cell by using the rayPosition, if there were no distortions, the ray would just travel one cell in a direction, and get the cell there,
				neighborCellsVector.push_back(aStarGrid.coordinatesWorldToCell(rayPosition));
				neighborCellsValidVector.push_back(true);
			}
		}
	}
}

