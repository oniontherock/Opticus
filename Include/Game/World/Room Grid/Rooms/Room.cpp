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



