#include "AStarGrid.hpp"


typedef std::vector<AStarCell> AStarGrid1D;
typedef std::vector<AStarGrid1D> AStarGrid2D;

AStarGrid::AStarGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<AStarCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cells = AStarGrid2D(gridSizeX, AStarGrid1D(gridSizeY, AStarCell()));

	for (uint32_t x = 0; x < gridSize.x; x++) {
		for (uint32_t y = 0; y < gridSize.y; y++) {
			cellSet(x, y, AStarCell(AStarCellPosition(x, y), cellsGetSize()));
		}
	}
}



void AStarGrid::cellUpdateNeighbors(CellCoordinate, CellCoordinate, DistortionGrid&) {
	//return cells[cellX][cellY].neighborCellsVectorUpdate(*this, cellSize, distortionGrid);
}
void AStarGrid::cellUpdateNeighbors(CellVector cellPos, DistortionGrid& distortionGrid) {
	return cellUpdateNeighbors(cellPos.x, cellPos.y, distortionGrid);
}
void AStarGrid::cellUpdateNeighborsFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, DistortionGrid& distortionGrid) {
	return cellUpdateNeighbors(coordinatesWorldToCell(worldX, worldY), distortionGrid);
}
void AStarGrid::cellUpdateNeighborsFromWorld(WorldVector worldPos, DistortionGrid& distortionGrid) {
	return cellUpdateNeighborsFromWorld(worldPos.x, worldPos.y, distortionGrid);
}

void AStarGrid::cellsAllUpdateNeighbors(DistortionGrid& distortionGrid) {
	for (uint32_t x = 0; x < gridSize.x; x++) {
		for (uint32_t y = 0; y < gridSize.y; y++) {
			cellUpdateNeighbors(x, y, distortionGrid);
		}
	}
}

