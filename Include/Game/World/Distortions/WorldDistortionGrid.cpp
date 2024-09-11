#include "WorldDistortionGrid.hpp"
#include <iostream>

WorldDistortionGrid::WorldDistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY, float distortionCellSizeX, float distortionCellSizeY) {

	gridSize = sf::Vector2u(gridSizeX, gridSizeY);

	distortionCellSize = sf::Vector2f(distortionCellSizeX, distortionCellSizeY);

	worldDistortionGrid = WorldDistortionGrid2D(gridSizeX, WorldDistortionGrid1D(gridSizeY, WorldDistortion()));
}

void WorldDistortionGrid::pixelSetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY, WorldDistortion distortion) {
	worldDistortionGrid[distortionCellX][distortionCellY] = distortion;
}

const WorldDistortion WorldDistortionGrid::pixelGetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY) {
	return worldDistortionGrid[distortionCellX][distortionCellY];
}

void WorldDistortionGrid::headingApplyDistortion(sf::Vector2f& heading, DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY) {
	worldDistortionGrid[distortionCellX][distortionCellY].headingApplyDistortion(heading);
}

void WorldDistortionGrid::headingApplyDistortion(sf::Vector2f& heading, DistortionCellVector distortionCell) {
	headingApplyDistortion(heading, distortionCell.x, distortionCell.x);
}

void WorldDistortionGrid::headingApplyDistortionSafe(sf::Vector2f& heading, DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY) {
	if (distortionCellX < 0 || distortionCellX >= gridSize.x || distortionCellY < 0 || distortionCellY >= gridSize.y) {
		
		std::cout << distortionCellX << " " << distortionCellY << std::endl;

		return;
	}

	headingApplyDistortion(heading, distortionCellX, distortionCellY);
}

void WorldDistortionGrid::headingApplyDistortionSafe(sf::Vector2f& heading, DistortionCellVector distortionCell) {
	headingApplyDistortionSafe(heading, distortionCell.x, distortionCell.x);
}


