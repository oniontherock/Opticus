#include "DistortionGrid.hpp"
#include <iostream>
#include <Auxiliary/ConsoleHandler.hpp>

DistortionGrid::DistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY) :
	Grid<Distortion>::Grid(gridSizeX, gridSizeY, distortionCellSizeX, distortionCellSizeY)
{
	cells = DistortionGrid2D(size_t(gridSizeX * distortionCellMultiplierX), DistortionGrid1D(size_t(gridSizeY * distortionCellMultiplierY), Distortion()));
}

