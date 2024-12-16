#include "DistortionGrid.hpp"
#include <iostream>
#include <Auxiliary/ConsoleHandler.hpp>

DistortionGrid::DistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY) :
	Grid<DistortionCell>::Grid(gridSizeX * distortionCellMultiplier, gridSizeY * distortionCellMultiplier, distortionCellSize, distortionCellSize)
{
	cells = DistortionGrid2D(size_t(gridSizeX * distortionCellMultiplier), DistortionGrid1D(size_t(gridSizeY * distortionCellMultiplier), DistortionCell()));
}

