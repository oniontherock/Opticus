#include "WorldDistortionGrid.hpp"
#include <iostream>

WorldDistortionGrid::WorldDistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY) {

	gridSize = sf::Vector2u(gridSizeX * distortionCellMultiplierX, gridSizeY * distortionCellMultiplierY);

	worldDistortionGrid = WorldDistortionGrid2D(gridSizeX * distortionCellMultiplierX, WorldDistortionGrid1D(gridSizeY * distortionCellMultiplierY, WorldDistortion()));
}

void WorldDistortionGrid::pixelSetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY, WorldDistortion distortion) {
	worldDistortionGrid[distortionCellX * distortionCellMultiplierX][distortionCellY * distortionCellMultiplierY] = distortion;
}

WorldDistortion& WorldDistortionGrid::pixelGetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY) {
	return worldDistortionGrid[distortionCellX * distortionCellMultiplierX][distortionCellY * distortionCellMultiplierY];
}


