#include "WorldGrid.hpp"



WorldGrid::WorldGrid(uint32_t worldSizeX, uint32_t worldSizeY) :
	imageGrid(WorldImageGrid(1, 1, float(worldSizeX), float(worldSizeY))),
	distortionGrid(WorldDistortionGrid(worldSizeX, worldSizeY, 1, 1))
{}
