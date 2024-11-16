#include "AStarPathfinder.hpp"
#include <set>
#include <queue>
#include <iostream>
#include <cmath>
#include <Auxiliary/ConsoleHandler.hpp>
#include "../../../ACECS/ECSRegistry.hpp"


AStarPath AStarPathfinder::pathRetrace(AStarCell& cellStart, AStarCell& cellEnd) {
	
	AStarPath path;
	// current cell in retrace, used for finding path
	AStarCell* cellBackTraceCur = &cellEnd;

	while (*cellBackTraceCur != cellStart) {
		path.push_back(cellBackTraceCur->cellPositionWorld);

		cellBackTraceCur = cellBackTraceCur->cellParent;
	}

	return path;
}

void AStarPathfinder::cellsResetData(std::vector<AStarCell*> cells) {
	// reset costs of all cells
	for (uint16_t i = 0; i < cells.size(); i++) {
		cells[i]->resetPathingData();
	}
}

AStarCostValue AStarPathfinder::cellDistanceGet(AStarCellPosition cellPosFrom, AStarCellPosition cellPosTo) {
	AStarCostValue distX = AStarCostValue(std::abs(int(cellPosFrom.x) - int(cellPosTo.x)));
	AStarCostValue distY = AStarCostValue(std::abs(int(cellPosFrom.y) - int(cellPosTo.y)));

	if (distX > distY) {
		return (14 * distY) + 10 * (distX - distY);
		}
	else {
		return (14 * distX) + 10 * (distY - distX);
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

AStarPath AStarPathfinder::pathGet(sf::Vector2f pointStart, sf::Vector2f pointEnd, AStarGrid& aStarGrid) {

	AStarCellPosition cellStart = aStarGrid.coordinatesWorldToCell(pointStart);
	AStarCellPosition cellEnd = aStarGrid.coordinatesWorldToCell(pointEnd);

	std::vector<AStarCell*> cellsOpenVector; // cells to be evaluated
	std::vector<AStarCellPosition> cellsClosedVector; // cells that have already been evaluated

	cellsOpenVector.push_back(&aStarGrid.cellGet(cellStart));

	AStarCell* cellCurrent = *cellsOpenVector.begin();

	while ((cellCurrent->cellPositionGrid != cellEnd) && (cellsOpenVector.size() > 0)) {

		cellCurrent = *cellsOpenVector.begin();
		for (auto itr = cellsOpenVector.begin(); itr != cellsOpenVector.end(); itr++) {

			if (**itr < *cellCurrent) {
				cellCurrent = *itr;
			}
		}

		auto itr = std::find(cellsOpenVector.begin(), cellsOpenVector.end(), cellCurrent);

		if (itr != cellsOpenVector.end()) {
			cellsOpenVector.erase(itr);
		}
		cellsClosedVector.push_back(cellCurrent->cellPositionGrid);

		for (int16_t x = -1; x <= 1; x++) {
			for (int16_t y = -1; y <= 1; y++) {

				if (x == 0 && y == 0) continue;

				if (!aStarGrid.cellPosIsInGrid(cellCurrent->cellPositionGrid.x + x, cellCurrent->cellPositionGrid.y + y)) continue;

				AStarCell& cellNeighbor = aStarGrid.cellGet(cellCurrent->cellPositionGrid.x + x, cellCurrent->cellPositionGrid.y + y);

				auto cellPosFind = [cellNeighbor](AStarCellPosition cellPos) { return cellPos == cellNeighbor.cellPositionGrid; };
				auto cellFind = [cellNeighbor](AStarCell* cell) { return *cell == cellNeighbor; };

				// skip current neighbor if they aren't valid or have already been explored
				if (!cellNeighbor.valid || std::find_if(cellsClosedVector.begin(), cellsClosedVector.end(), cellPosFind) != cellsClosedVector.end()) continue;


				// potential new costG for neighbor if path is shorter than existing neighborCur costG
				AStarCostValue neighborNewCostG = cellCurrent->costG + cellDistanceGet(cellCurrent->cellPositionGrid, cellNeighbor.cellPositionGrid);

				// whether the neighborCur is in the cellsOpenVector
				bool neighborIsInOpen = std::find_if(cellsOpenVector.begin(), cellsOpenVector.end(), cellFind) != cellsOpenVector.end();

				if (neighborNewCostG < cellNeighbor.costG || !neighborIsInOpen) {

					cellNeighbor.costG = neighborNewCostG;
					cellNeighbor.costH = cellDistanceGet(cellNeighbor.cellPositionGrid, cellEnd);
					cellNeighbor.costF = cellNeighbor.costG + cellNeighbor.costH;
					cellNeighbor.cellParent = cellCurrent;

					if (!neighborIsInOpen) {
						cellsOpenVector.push_back(&aStarGrid.cellGet(cellNeighbor.cellPositionGrid));
					}
				}
			}
		}
	}

	// get finished path
	AStarPath path = pathRetrace(aStarGrid.cellGet(cellStart), aStarGrid.cellGet(cellEnd));

	// get all cells
	std::vector<AStarCell*> cellsAll;
	cellsAll.reserve(cellsOpenVector.size() + cellsClosedVector.size());
	cellsAll.insert(cellsAll.end(), cellsOpenVector.begin(), cellsOpenVector.end());
	for (uint16_t i = 0; i < cellsClosedVector.size(); i++) {
		cellsAll.push_back(&aStarGrid.cellGet(cellsClosedVector[i]));
	}
	// reset data for all cells we modified
	cellsResetData(cellsAll);

	return path;
}

AStarPath AStarPathfinder::pathGet(sf::Vector2f pointEnd, Entity& entity) {
	if (!entity.entityComponentHas<EntityComponents::ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("AStarPathfinder \"pathGet\" function failed: Exception: entity does not have ComponentPosition");
		return AStarPath();
	}

	auto* entityComponentPosition = entity.entityComponentGet<EntityComponents::ComponentPosition>();

	sf::Vector2f pointStart = entityComponentPosition->position;

	AStarGrid& aStarGrid = GameLevelGrid::levelGet(entityComponentPosition->worldPosition.level)->aStarGrid;

	return pathGet(pointStart, pointEnd, aStarGrid);
}
AStarPath AStarPathfinder::pathGet(sf::Vector2f pointEnd, EntityId entityId) {
	pathGet(pointEnd, EntityManager::entityGet(entityId));
}
