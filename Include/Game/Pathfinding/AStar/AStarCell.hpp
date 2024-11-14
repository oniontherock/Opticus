#ifndef __A_STAR_CELL_H__
#define __A_STAR_CELL_H__

#include <SFML/System/Vector2.hpp>
#include <vector>
#include "../../World/Distortions/WorldDistortionGrid.hpp"

// position of a cell in the AStarGrid
typedef sf::Vector2u AStarCellPosition;
// vector of AStarCellPositions, used in AStarCell for listing neighbors,
// 
// cells are ordered like so:
// 
// [0][1][2]
// [3][4][5]
// [6][7][8]
//
// note the fact that a cell's own position is included, this is just so iteration around surrounding cells is easier
typedef std::vector<AStarCellPosition> NeighborCellsVector;

// type used for costs
typedef uint16_t AStarCostValue;

class AStarGrid;

class AStarCell {

	AStarCell();
	AStarCell(AStarCellPosition _cellPositionGrid, sf::Vector2f aStarGridCellSize);

	// whether this cell is valid for pathfinding, if not, the cell is treated like a wall
	bool valid = true;
	

	// position of the cell in the world
	sf::Vector2f cellPositionWorld;
	// position of the cell in the AStarGrid
	AStarCellPosition cellPositionGrid;

	AStarCellPosition cellParentPositionGrid;


	// g cost of the AStarCell
	AStarCostValue costG;
	// h cost of the AStarCell
	AStarCostValue costH;
	// f cost of the AStarCell
	AStarCostValue costF;
	void costHCalculate(AStarCellPosition targetPosition);

	// reset G, H, and F costs,
	void resetCosts();
	// resets all data that is modified during path generation,
	// this includes:
	// 
	// costG,
	// costH,
	// costF,
	void resetPathingData();

public:

	bool operator< (const AStarCell& other) const;
	bool operator== (const AStarCell& other) const;

	friend class AStarGrid;
};


#endif