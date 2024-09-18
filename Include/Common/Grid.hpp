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
	// size of the whole grid, including the size of the cells
	sf::Vector2f gridSizeFull;

public:

	Grid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
		gridSize(sf::Vector2u(gridSizeX, gridSizeY)),
		cellCount(gridSizeX * gridSizeY),
		cellSize(sf::Vector2f(cellSizeX, cellSizeY)),
		gridSizeFull(sf::Vector2f(gridSizeX * cellSize.x, gridSizeY * cellSize.y))
	{}
	Grid(uint32_t gridSizeX, uint32_t gridSizeY) :
		Grid(gridSizeX, gridSizeY, 1.f, 1.f)
	{}

	CellVector coordinatesWorldToCell(WorldCoordinate worldX, WorldCoordinate worldY) {
		return CellVector(worldX / cellSize.x, worldY / cellSize.y);
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
		CellVector cellPos = coordinatesWorldToCell(worldX, worldY);
		cells[cellPos.x][cellPos.y] = cell;
	}
	void cellSetFromWorld(WorldVector worldPos, C cell) {
		CellVector cellPos = coordinatesWorldToCell(worldPos);
		cells[cellPos.x][cellPos.y] = cell;
	}

	bool cellPosIsInGrid(CellCoordinate cellX, CellCoordinate cellY) {
		return !(cellX < 0 || cellX >= gridSize.x || cellY < 0 || cellY >= gridSize.y);
	}
	bool cellPosIsInGrid(CellVector cellPos) {
		return cellPosIsInGrid(cellPos.x, cellPos.y);
	}
	bool worldPosIsInGrid(WorldCoordinate worldX, WorldCoordinate worldY) {
		return cellPosIsInGrid(coordinatesWorldToCell(worldX, worldY));
	}
	bool worldPosIsInGrid(WorldVector worldPos) {
		return worldPosIsInGrid(worldPos.x, worldPos.y);
	}

	bool worldPosIsInGridFull(WorldCoordinate worldX, WorldCoordinate worldY) {
		return !(worldX < 0 || worldX >= gridSizeFull.x || worldY < 0 || worldY >= gridSizeFull.y);
	}
	bool worldPosIsInGridFull(WorldVector worldPos) {
		return worldPosIsInGridFull(worldPos.x, worldPos.y);
	}
	bool cellPosIsInGridFull(CellCoordinate cellX, CellCoordinate cellY) {
		return worldPosIsInGridFull(coordinatesCellToWorld(cellX, cellY));
	}
	bool cellPosIsInGridFull(CellVector cellPos) {
		return worldPosIsInGridFull(coordinatesCellToWorld(cellPos));
	}
};


#endif