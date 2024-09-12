#include "WorldDistortionGrid.hpp"
#include <iostream>

WorldDistortionGrid::WorldDistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY) {

	gridSize = sf::Vector2u(size_t(gridSizeX * distortionCellMultiplierX), size_t(gridSizeY * distortionCellMultiplierY));

	worldDistortionGrid = WorldDistortionGrid2D(size_t(gridSizeX * distortionCellMultiplierX), WorldDistortionGrid1D(size_t(gridSizeY * distortionCellMultiplierY), WorldDistortion()));
}

void WorldDistortionGrid::pixelSetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY, WorldDistortion distortion) {
	worldDistortionGrid[size_t(distortionCellX * distortionCellMultiplierX)][size_t(distortionCellY * distortionCellMultiplierY)] = distortion;
}
void WorldDistortionGrid::pixelSetDistortionSafe(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY, WorldDistortion distortion) {

	size_t x = size_t(distortionCellX * distortionCellMultiplierX);
	size_t y = size_t(distortionCellY * distortionCellMultiplierY);

	if (x < 0 || x >= gridSize.x || y < 0 || y >= gridSize.y) return;

	worldDistortionGrid[x][y] = distortion;
}

WorldDistortion& WorldDistortionGrid::pixelGetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY) {
	return worldDistortionGrid[size_t(distortionCellX * distortionCellMultiplierX)][size_t(distortionCellY * distortionCellMultiplierY)];
}


