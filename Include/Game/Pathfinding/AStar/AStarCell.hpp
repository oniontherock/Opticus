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

	// vector of neighboring cells, note that these may or may not be the actual neighboring cells, since a distortion could make a cell neighbor a different cell
	NeighborCellsVector neighborCellsVector;
	// same as neighborCellsVector but contains bools indicating whether a neighbor is at the given position
	std::vector<bool> neighborCellsValidVector;

	

	// position of the cell in the world
	sf::Vector2f cellPositionWorld;
	// position of the cell in the AStarGrid
	AStarCellPosition cellPositionGrid;


	// axis from this cell to the cell's parent in a path
	sf::Vector2f cellParentFromAxis;
	// index of the parent in the neighborCellsVector
	uint8_t cellParentNeighborIndex;
	// g cost of the AStarCell
	AStarCostValue costG;
	// h cost of the AStarCell
	AStarCostValue costH;
	// f cost of the AStarCell
	AStarCostValue costF;
	void costHCalculate();

	// reset G, H, and F costs,
	void resetCosts();
	// resets all data that is modified during path generation,
	// this includes:
	// 
	// cellParentFromAxis,
	// cellParentNeighborInd,
	// costG,
	// costH,
	// costF,
	void resetPathingData();

	// update the neighborCellsVector,
	// @param aStarGrid: grid of AStarCells, used for finding neighbors.
	// @param aStarGridCellSize: size of cells in the aStarGrid.
	// @param distortionGrid: distortionGrid, used for applying distortions to the neighbor search
	void neighborCellsVectorUpdate(AStarGrid& aStarGrid, sf::Vector2f aStarGridCellSize, WorldDistortionGrid& distortionGrid);

public:

	bool operator< (const AStarCell& other) const;
	bool operator== (const AStarCell& other) const;

	friend class AStarGrid;
};


#endif