#ifndef __WORLD_DISTORTION_GRID_H__
#define __WORLD_DISTORTION_GRID_H__

#include "../../../Common/Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include "WorldDistortion.hpp"
#include <functional>
#include <string>
#include <vector>

typedef std::vector<WorldDistortion> WorldDistortionGrid1D;
typedef std::vector<WorldDistortionGrid1D> WorldDistortionGrid2D;

typedef uint32_t DistortionCellCoordinate;
typedef sf::Vector2u DistortionCellVector;

struct WorldDistortionGrid : Grid<WorldDistortion> {

	WorldDistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY);

	// the size of the distortion cells.
	// with a size of 1x1, the distortion cells will be the size of pixels.
	// larger values are generally faster, but they decrease the resolution of the worldDistortionGrid
	static constexpr uint32_t distortionCellSizeX = 4;
	static constexpr uint32_t distortionCellSizeY = 4;

	// multiplier for distortionCell coordinates
	static constexpr float distortionCellMultiplierX = 1.f / distortionCellSizeX;
	static constexpr float distortionCellMultiplierY = 1.f / distortionCellSizeY;
};

#endif
