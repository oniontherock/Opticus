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


