#include "WorldDistortionGrid.hpp"
#include <iostream>
#include <Auxiliary/ConsoleHandler.hpp>

WorldDistortionGrid::WorldDistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY) :
	Grid<WorldDistortion>::Grid(gridSizeX, gridSizeY, distortionCellSizeX, distortionCellSizeY)
{
	cells = WorldDistortionGrid2D(size_t(gridSizeX * distortionCellMultiplierX), WorldDistortionGrid1D(size_t(gridSizeY * distortionCellMultiplierY), WorldDistortion()));
}

