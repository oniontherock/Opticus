#include "Room.hpp"



Room::Room(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<RoomCell>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cellsInitialize();
}
Room::Room(uint32_t gridSizeX, uint32_t gridSizeY) :
	Grid<RoomCell>::Grid(gridSizeX, gridSizeY, 1.f, 1.f)
{
	cellsInitialize();
}

void Room::cellsSetData() {
	for (uint32_t x = 0; x < gridSize.x; x++) {
		for (uint32_t y = 0; y < gridSize.y; y++) {
			cells[x][y].worldPos = positionWorld.position + sf::Vector2f(float(x) * cellSize.x, float(y) * cellSize.y);
		}
	}
}




