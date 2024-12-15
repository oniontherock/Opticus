#ifndef __DISTORTION_GRID_H__
#define __DISTORTION_GRID_H__

#include "../../../Common/Grid.hpp"
#include "Distortion.hpp"
#include "SFML/System/Vector2.hpp"
#include <functional>
#include <string>
#include <vector>
#include "DistortionCell.hpp"

typedef std::vector<DistortionCell> DistortionGrid1D;
typedef std::vector<DistortionGrid1D> DistortionGrid2D;

typedef uint32_t DistortionCellCoordinate;
typedef sf::Vector2u DistortionCellVector;

struct DistortionGrid : Grid<DistortionCell> {

	friend std::ofstream& operator<<(std::ofstream& str, DistortionGrid& item);
	friend std::ifstream& operator>>(std::ifstream& str, DistortionGrid& item);

	DistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY);

	// the size of the distortion cells.
	// with a size of 1x1, the distortion cells will be the size of pixels.
	// larger values are generally faster, but they decrease the resolution of the DistortionGrid
	static constexpr uint32_t distortionCellSize = 8;

	// multiplier for distortionCell coordinates
	static constexpr float distortionCellMultiplier = 1.f / distortionCellSize;
};

#endif
