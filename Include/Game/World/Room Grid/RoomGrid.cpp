#include "RoomGrid.hpp"

RoomGrid::RoomGrid(uint32_t gridSizeX, uint32_t gridSizeY, uint32_t roomCellCountX, uint32_t roomCellCountY, float roomCellSizeX, float roomCellSizeY) :
	Grid<Room>::Grid(gridSizeX, gridSizeY, roomCellCountX * roomCellSizeX, roomCellCountY * roomCellSizeY)
{
	
	cellsInitialize(roomCellCountX, roomCellCountY, roomCellSizeX, roomCellSizeY);

	for (uint32_t x = 0; x < gridSize.x; x++) {
		for (uint32_t y = 0; y < gridSize.y; y++) {

			cells[x][y].positionGrid = RoomGridPosition(x, y);
			cells[x][y].positionWorld = WorldPosition(0, 0, 0, x * (roomCellCountX * roomCellSizeX), y * (roomCellCountY * roomCellSizeY));
			cells[x][y].cellsSetData();

			ConsoleHandler::consolePrintLoadingGame("Room finished initialization: (" + std::to_string(x) + ", " + std::to_string(y) + ")");
		}
	}
}
