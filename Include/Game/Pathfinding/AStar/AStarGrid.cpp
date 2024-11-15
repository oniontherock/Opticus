#include "AStarGrid.hpp"
#include <set>
#include <queue>
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
struct AStarCellComparator {
	bool operator() (AStarCell* a, AStarCell* b) {
		return *b < *a;
	}
};

struct CellHeap : std::priority_queue<AStarCell*, std::vector<AStarCell*>, AStarCellComparator> {
public:

	bool has(const AStarCell& val) const {
		auto first = this->c.cbegin();
		auto last = this->c.cend();
		while (first != last) {
			if (**first == val) return true;
			first++;
		}
		return false;
	}
};

struct SetComparator {
	bool operator() (const AStarCellPosition& a, const AStarCellPosition& b) const {
		return (a.x < b.x) && (a.y < b.y);
	}
};

AStarPath AStarGrid::pointsGetPath(sf::Vector2f pointA, sf::Vector2f pointB) {
	
	AStarCellPosition cellStart = coordinatesWorldToCell(pointA);
	AStarCellPosition cellEnd = coordinatesWorldToCell(pointB);

	CellHeap cellsOpenVector; // cells to be evaluated

	std::vector<AStarCellPosition> cellsClosedVector; // cells that have already been evaluated

	cellsOpenVector.emplace(&cellGet(cellStart));

	AStarCell* cellCurrent = cellsOpenVector.top();

	while ((cellCurrent->cellPositionGrid != cellEnd) && (cellsOpenVector.size() > 0)) {

		cellCurrent = cellsOpenVector.top();
		cellsOpenVector.pop();
		//std::cout << cellCurrent->costF << std::endl;
		cellsClosedVector.push_back(cellCurrent->cellPositionGrid);
		//std::cout << "burger" << std::endl;

		for (int16_t x = -1; x <= 1; x++) {
			for (int16_t y = -1; y <= 1; y++) {

				if (x == 0 && y == 0) continue;

				if (!cellPosIsInGrid(cellCurrent->cellPositionGrid.x + x, cellCurrent->cellPositionGrid.y + y)) continue;

				//sf::Vector2i axisToNeighbor = sf::Vector2i(x, y);

				AStarCell& cellNeighbor = cellGet(cellCurrent->cellPositionGrid.x + x, cellCurrent->cellPositionGrid.y + y);

				auto cellFind = [cellNeighbor](AStarCellPosition cellPos) { return cellPos == cellNeighbor.cellPositionGrid; };

				// skip current neighbor if they aren't valid or have already been explored
				if (!cellNeighbor.valid || std::find_if(cellsClosedVector.begin(), cellsClosedVector.end(), cellFind) != cellsClosedVector.end()) continue;

				cellNeighbor.costHCalculate(cellEnd);
				// euclidean distance from the cellCurrent to the cellNeighbor,
				// note that we don't square x or y because they are always either 1 or 0,
				AStarCostValue distanceEuclidean = AStarCostValue(sqrt((x * x) + (y * y)) * 10);

				// potential new costG for neighbor if path is shorter than existing neighborCur costG
				AStarCostValue neighborNewCostG = cellCurrent->costG + distanceEuclidean;

				AStarCostValue neighborNewCostF = neighborNewCostG + cellNeighbor.costH;

				// whether the neighborCur is in the cellsOpenVector
				bool neighborIsInOpen = cellsOpenVector.has(cellNeighbor);

				if (neighborNewCostF < cellNeighbor.costF || !neighborIsInOpen) {
					if (!neighborIsInOpen) {
						cellsOpenVector.emplace(&cellGet(cellNeighbor.cellPositionGrid));
					}
					cellNeighbor.costF = neighborNewCostF;

					cellNeighbor.cellParentPositionGrid = cellCurrent->cellPositionGrid;
				}
			}
		}
	}

	AStarPath path;

	// current cell in back tracer, used for finding path
	AStarCell* cellBackTraceCur = &cellGet(cellEnd);

	while (cellBackTraceCur->cellPositionGrid != cellStart) {
		path.push_back(coordinatesCellToWorld(cellBackTraceCur->cellPositionGrid) + (cellsGetSize() / 2.f));

		cellBackTraceCur = &cellGet(cellBackTraceCur->cellParentPositionGrid);
	}

	// reset costs of all cells in the open vector
	while (!cellsOpenVector.empty()) {
		cellsOpenVector.top()->resetPathingData();
		cellsOpenVector.pop();
	}
	// reset costs of all cells in the closed vector
	while (cellsClosedVector.size() > 0) {
		cellGet(*cellsClosedVector.begin()).resetPathingData();
		cellsClosedVector.erase(cellsClosedVector.begin());
	}

	return path;
}


void AStarGrid::cellUpdateNeighbors(CellCoordinate, CellCoordinate, WorldDistortionGrid&) {
	//return cells[cellX][cellY].neighborCellsVectorUpdate(*this, cellSize, distortionGrid);
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

