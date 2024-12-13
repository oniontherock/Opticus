#ifndef __A_STAR_GRID_H__
#define __A_STAR_GRID_H__

#include "../../../../Common/Grid.hpp"
#include "AStarCell.hpp"
#include <SFML/System/Vector2.hpp>
#include <vector>

typedef std::vector<sf::Vector2f> AStarPath;

class AStarGrid : public Grid<AStarCell> {
public:

	friend std::ofstream& operator<<(std::ofstream& str, AStarGrid& item);
	friend std::ifstream& operator>>(std::ifstream& str, AStarGrid& item);

	AStarGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	void cellUpdateNeighbors(CellCoordinate cellX, CellCoordinate cellY, DistortionGrid& distortionGrid);
	void cellUpdateNeighbors(CellVector cellPos, DistortionGrid& distortionGrid);
	void cellUpdateNeighborsFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, DistortionGrid& distortionGrid);
	void cellUpdateNeighborsFromWorld(WorldVector worldPos, DistortionGrid& distortionGrid);

	void cellsAllUpdateNeighbors(DistortionGrid& distortionGrid);
	
};



#endif
