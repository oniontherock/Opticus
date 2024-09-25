#include "WorldImageGrid.hpp"


WorldImageGrid::WorldImageGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<WorldTexture>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	WorldImageGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		WorldImageGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y].create(uint32_t(cellSize.x), uint32_t(cellSize.y));
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}


void WorldImageGrid::imagesAllReadSet(const bool state) {
	for (uint16_t x = 0; x < gridSize.x; x++) {
		for (uint16_t y = 0; y < gridSize.y; y++) {
			cells[x][y].read = state;
		}
	}
}
void WorldImageGrid::imagesAllWriteSet(const bool state) {
	for (uint16_t x = 0; x < gridSize.x; x++) {
		for (uint16_t y = 0; y < gridSize.y; y++) {
			cells[x][y].write = state;
		}
	}
}
