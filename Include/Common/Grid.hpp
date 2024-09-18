#ifndef __GRID_H__
#define __GRID_H__

#include <vector>
#include <SFML/System/Vector2.hpp>

template <class C>
class Grid {
protected:

	using CellCoordinate = uint32_t;
	using CellVector = sf::Vector2<CellCoordinate>;
	using WorldCoordinate = float;
	using WorldVector = sf::Vector2<WorldCoordinate>;

	// 2d array of cells
	std::vector<std::vector<C>> cells;
	// amount of cells in the grid in the x and y axes
	sf::Vector2u gridSize;
	// total amount of cells in the grid
	uint32_t cellCount;
	// size of cells
	sf::Vector2f cellSize;
public:

	CellVector coordinatesWorldToCell(WorldCoordinate worldX, WorldCoordinate worldY) {
		return WorldVector(worldX / cellSize.x, worldY / cellSize.y);
	}
	CellVector coordinatesWorldToCell(WorldVector worldPos) {
		return coordinatesWorldToCell(worldPos.x, worldPos.y);
	}
	
	WorldVector coordinatesCellToWorld(CellCoordinate cellX, CellCoordinate cellY) {
		return WorldVector(cellX * cellSize.x, cellY * cellSize.y);
	}
	WorldVector coordinatesCellToWorld(CellVector cellPos) {
		return coordinatesCellToWorld(cellPos.x, cellPos.y);
	}


	C& cellGet(CellCoordinate cellX, CellCoordinate cellY) {
		return cells[cellX][cellY];
	}
	C& cellGet(CellVector cellPos) {
		return cellGet(cellPos.x, cellPos.y);
	}
	C& cellGetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
		return cellGet(coordinatesWorldToCell(worldX, worldY));
	}
	C& cellGetFromWorld(WorldVector worldPos) {
		return cellGetFromWorld(worldPos.x, worldPos.y);
	}

	void cellSet(CellCoordinate cellX, CellCoordinate cellY, C cell) {
		cells[cellX][cellY] = cell;
	}
	void cellSet(CellVector cellPos, C cell) {
		cells[cellPos.x][cellPos.y] = cell;
	}
	void cellSetFromWorld(WorldCoordinate worldX, WorldCoordinate worldY, C cell) {
		
		CellCoordinate cellX = worldX / cellSize.x;
		CellCoordinate cellY = worldY / cellSize.y;

		cells[cellX][cellY] = cell;
	}
	void cellSetFromWorld(WorldVector worldPos, C cell) {

		CellCoordinate cellX = worldPos.x / cellSize.x;
		CellCoordinate cellY = worldPos.y / cellSize.y;

		cells[cellX][cellY] = cell;
	}


};


#endif