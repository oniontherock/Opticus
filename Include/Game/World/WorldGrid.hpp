#ifndef __WORLD_GRID_H__
#define __WORLD_GRID_H__

#include "Image Grid/WorldImageGrid.hpp"
#include "Distortions/WorldDistortionGrid.hpp"

// handles everything related to the various grid objects, like the WorldDistortionGrid or the WorldImageGrid.
// doesn't directly hold any grid data itself, but it holds objects that hold grids, like the previously mentioned WorldDistortionGrid or WorldImageGrid.
class WorldGrid {
public:

	WorldDistortionGrid distortionGrid;
	WorldImageGrid imageGrid;

	WorldGrid(uint32_t worldSizeX, uint32_t worldSizeY);
};

#endif