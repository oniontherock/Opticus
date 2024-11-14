#ifndef __A_STAR_GRID_H__
#define __A_STAR_GRID_H__

#include "../../../Common/Grid.hpp"
#include "AStarCell.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

typedef std::vector<sf::Vector2f> AStarPath;

class AStarGrid : public Grid<AStarCell> {


public:
	AStarGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
	
	AStarPath pointsGetPath(sf::Vector2f pointA, sf::Vector2f pointB);
	
	void cellUpdateNeighbors(CellCoordinate cellX, CellCoordinate cellY, WorldDistortionGrid& distortionGrid);
	void cellUpdateNeighbors(CellVector cellPos, WorldDistortionGrid& distortionGrid);
	void cellUpdateNeighborsFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, WorldDistortionGrid& distortionGrid);
	void cellUpdateNeighborsFromWorld(WorldVector worldPos, WorldDistortionGrid& distortionGrid);

	void cellsAllUpdateNeighbors(WorldDistortionGrid& distortionGrid);
	
};



#endif
