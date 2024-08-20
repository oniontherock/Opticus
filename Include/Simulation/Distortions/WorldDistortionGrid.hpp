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
typedef sf::Vector2i DistortionCellVector;

struct WorldDistortionGrid {
	static sf::Vector2f distortionCellSize;
	static WorldDistortionGrid2D worldDistortionGrid;

	static void worldDistortionGridInitialize(uint32_t gridSizeX, uint32_t gridSizeY, float distortionCellSizeX, float distortionCellSizeY);

	static void pixelSetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY, WorldDistortion distortion);
	static const WorldDistortion pixelGetDistortion(DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY);

	static void headingApplyDistortion(sf::Vector2f& heading, DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY);
	static void headingApplyDistortion(sf::Vector2f& heading, DistortionCellVector distortionCell);

	static void headingApplyDistortionSafe(sf::Vector2f& heading, DistortionCellCoordinate distortionCellX, DistortionCellCoordinate distortionCellY);
	static void headingApplyDistortionSafe(sf::Vector2f& heading, DistortionCellVector distortionCell);

private:
	static sf::Vector2i gridSize;
};

#endif
