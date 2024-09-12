#include "WorldDistortionGrid.hpp"
#include <iostream>

WorldDistortionGrid::WorldDistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY) {

	gridSize = sf::Vector2u(size_t(gridSizeX * distortionCellMultiplierX), size_t(gridSizeY * distortionCellMultiplierY));

	worldDistortionGrid = WorldDistortionGrid2D(size_t(gridSizeX * distortionCellMultiplierX), WorldDistortionGrid1D(size_t(gridSizeY * distortionCellMultiplierY), WorldDistortion()));
}

void WorldDistortionGrid::pixelSetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY, WorldDistortion distortion) {
	worldDistortionGrid[size_t(distortionCellX * distortionCellMultiplierX)][size_t(distortionCellY * distortionCellMultiplierY)] = distortion;
}

WorldDistortion& WorldDistortionGrid::pixelGetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY) {
	return worldDistortionGrid[size_t(distortionCellX * distortionCellMultiplierX)][size_t(distortionCellY * distortionCellMultiplierY)];
}


