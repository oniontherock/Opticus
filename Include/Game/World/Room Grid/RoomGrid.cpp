#include "RoomGrid.hpp"


RoomGrid::RoomGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<Room>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	cellsInitialize(32, 32, 16.f, 16.f);
}

RoomGrid::RoomGrid(uint32_t gridSizeX, uint32_t gridSizeY) :
	Grid<Room>::Grid(gridSizeX, gridSizeY, 1.f, 1.f)
{
	cellsInitialize(32, 32, 16.f, 16.f);
}

