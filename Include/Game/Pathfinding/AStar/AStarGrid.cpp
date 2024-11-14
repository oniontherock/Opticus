#include "AStarGrid.hpp"
#include <set>
#include <iostream>
#include <Auxiliary/ConsoleHandler.hpp>

typedef std::vector<AStarCell> AStarGrid1D;
typedef std::vector<AStarGrid1D> AStarGrid2D;

AStarGrid::AStarGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<AStarCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = AStarGrid2D(gridSizeX, AStarGrid1D(gridSizeY, AStarCell()));


	for (uint32_t x = 0; x < gridSize.x; x++) {
		for (uint32_t y = 0; y < gridSize.y; y++) {
			cellSet(x, y, AStarCell(AStarCellPosition(x, y), cellSize));
		}
	}
}

AStarPath AStarGrid::pointsGetPath(sf::Vector2f pointA, sf::Vector2f pointB) {
	
	AStarCell& cellStart = cellGetFromWorld(pointA);
	AStarCell& cellEnd = cellGetFromWorld(pointB);

	std::vector<AStarCell*> cellsOpenVector; // cells to be evaluated
	std::vector<AStarCell*> cellsClosedVector; // cells that have already been evaluated

	cellsOpenVector.push_back(&cellStart);

	AStarCell* cellCurrent = &cellStart;


	while ((cellCurrent->cellPositionGrid != cellEnd.cellPositionGrid) && (cellsOpenVector.size() > 0)) {
		
		auto cellsOpenVectorMinItr = std::min_element(cellsOpenVector.begin(), cellsOpenVector.end());

		cellCurrent = *cellsOpenVectorMinItr;
		cellsOpenVector.erase(cellsOpenVectorMinItr);
		cellsClosedVector.push_back(cellCurrent);

		for (int16_t x = -1; x <= 1; x++) {
			for (int16_t y = -1; y <= 1; y++) {

				if (x == 0 && y == 0) continue;

				if (!cellPosIsInGrid(cellCurrent->cellPositionGrid.x + x, cellCurrent->cellPositionGrid.y + y)) continue;

				sf::Vector2i axisToNeighbor = sf::Vector2i(x, y);

				AStarCell& cellNeighbor = cellGet(cellCurrent->cellPositionGrid.x + x, cellCurrent->cellPositionGrid.y + y);

				auto referenceWrapperEquals = [cellNeighbor](const AStarCell* a) {
					return *a == cellNeighbor;
					};

				// skip current neighbor if they aren't valid or have already been explored
				if ((!cellNeighbor.valid) || (std::find_if(cellsClosedVector.begin(), cellsClosedVector.end(), referenceWrapperEquals) != cellsClosedVector.end())) continue;

				// euclidean distance from the cellCurrent to the cellNeighbor,
				// note that we don't square x or y because they are always either 1 or 0,
				AStarCostValue distanceEuclidean = AStarCostValue(sqrt(axisToNeighbor.x + axisToNeighbor.y) * 10.f);

				// potential new costG for neighbor if path is shorter than existing neighborCur costG
				AStarCostValue neighborNewCostG = cellCurrent->costG + distanceEuclidean;

				// whether the neighborCur is in the cellsOpenVector
				bool neighborIsInOpen = std::find_if(cellsOpenVector.begin(), cellsOpenVector.end(), referenceWrapperEquals) != cellsOpenVector.end();

				if (neighborNewCostG < cellNeighbor.costG || !neighborIsInOpen) {
					if (!neighborIsInOpen) {
						cellNeighbor.costHCalculate();

						cellsOpenVector.push_back(&cellNeighbor);
					}
					cellNeighbor.costF = neighborNewCostG + cellNeighbor.costH;

					// set the cellNeighbor's parent axis to be the axis from the current cell to the neighbor in world coordinates
					sf::Vector2f axisToNeighborNormalized = Vector2fMath::normalize(float(axisToNeighbor.x), float(axisToNeighbor.y));;
					//cellNeighbor.cellParentFromAxis = sf::Vector2f(axisToNeighborNormalized.x * cellsGetSizeX(), axisToNeighborNormalized.y * cellsGetSizeY());
					// set the cellNeighbor's parent neighbor index to be the opposite of the neighbor index from the current cell to the neighbor
					//cellNeighbor.cellParentNeighborIndex = (8u - i);

				}
			}
		}
	}

	std::cout << cellEnd.cellParentNeighborIndex << std::endl;
	AStarPath path;

	// current cell in back tracer, used for finding path
	AStarCell* cellBackTraceCur = &cellEnd;
	// current position in back trace.
	// 
	// we use this and not just the cell's position because a cell's position, and the connection could be different,
	// because if you travel through a portal, that teleports you down by 50 cells, then using the cell position would cause you to go down,
	// but you actually travel to the right to go through the portal
	sf::Vector2f positionBackTraceCur = cellBackTraceCur->cellPositionWorld;

	while (cellBackTraceCur->cellParentFromAxis.x != 0 || cellBackTraceCur->cellParentFromAxis.y != 0) {
		path.push_back(positionBackTraceCur);

		positionBackTraceCur -= cellBackTraceCur->cellParentFromAxis;
		cellBackTraceCur = &cellGet(cellBackTraceCur->neighborCellsVector[cellBackTraceCur->cellParentNeighborIndex]);

	}

	// reset costs of all cells in the open vector
	for (uint16_t i = 0; i < cellsOpenVector.size(); i++) {
		cellsOpenVector[i]->resetCosts();
	}
	// reset costs of all cells in the closed vector
	for (uint16_t i = 0; i < cellsClosedVector.size(); i++) {
		cellsClosedVector[i]->resetCosts();
	}

	return path;
}


void AStarGrid::cellUpdateNeighbors(CellCoordinate cellX, CellCoordinate cellY, WorldDistortionGrid & distortionGrid) {
	return cells[cellX][cellY].neighborCellsVectorUpdate(*this, cellSize, distortionGrid);
}
void AStarGrid::cellUpdateNeighbors(CellVector cellPos, WorldDistortionGrid& distortionGrid) {
	return cellUpdateNeighbors(cellPos.x, cellPos.y, distortionGrid);
}
void AStarGrid::cellUpdateNeighborsFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, WorldDistortionGrid& distortionGrid) {
	return cellUpdateNeighbors(coordinatesWorldToCell(worldX, worldY), distortionGrid);
}
void AStarGrid::cellUpdateNeighborsFromWorld(WorldVector worldPos, WorldDistortionGrid& distortionGrid) {
	return cellUpdateNeighborsFromWorld(worldPos.x, worldPos.y, distortionGrid);
}

void AStarGrid::cellsAllUpdateNeighbors(WorldDistortionGrid& distortionGrid) {
	for (uint32_t x = 0; x < gridSize.x; x++) {
		for (uint32_t y = 0; y < gridSize.y; y++) {
			cellUpdateNeighbors(x, y, distortionGrid);
		}
	}
}

