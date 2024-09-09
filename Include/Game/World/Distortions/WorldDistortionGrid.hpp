#ifndef __WORLD_DISTORTION_GRID_H__
#define __WORLD_DISTORTION_GRID_H__

#include "DistortionFunctions.hpp"
#include "SFML/System/Vector2.hpp"
#include <functional>
#include <string>
#include <vector>

typedef std::vector<WorldDistortion> WorldDistortionGrid1D;
typedef std::vector<WorldDistortionGrid1D> WorldDistortionGrid2D;

typedef uint32_t DistortionCellCoordinate;
typedef sf::Vector2u DistortionCellVector;

struct WorldDistortionGrid {
	sf::Vector2f distortionCellSize;
	WorldDistortionGrid2D worldDistortionGrid;

	WorldDistortionGrid(uint32_t gridSizeX, uint32_t gridSizeY, float distortionCellSizeX, float distortionCellSizeY);

	void pixelSetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY, WorldDistortion distortion);
	const WorldDistortion pixelGetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY);

	void headingApplyDistortion(sf::Vector2f& heading, DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY);
	void headingApplyDistortion(sf::Vector2f& heading, DistortionCellVector distortionCell);

	void headingApplyDistortionSafe(sf::Vector2f& heading, DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY);
	void headingApplyDistortionSafe(sf::Vector2f& heading, DistortionCellVector distortionCell);

private:
	sf::Vector2u gridSize;
};

#endif
