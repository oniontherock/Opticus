#include "ObjectGrid.hpp"


ObjectGrid::ObjectGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<ObjectCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	ObjectGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		ObjectGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y] = ObjectCell();
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}

