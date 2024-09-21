#ifndef __ROOM_GRID_H__
#define __ROOM_GRID_H__

#include "../../../Common/Grid.hpp"
#include "Rooms/Room.hpp"
#include <cstdint>
#include <Auxiliary/ConsoleHandler.hpp>
#include <World/LevelTypeDefinitions.hpp>

class RoomGrid : public Grid<Room> {
public:
	// initialize rooms and initialize those rooms' cells
	RoomGrid(uint32_t gridSizeX, uint32_t gridSizeY, uint32_t roomCellCountX, uint32_t roomCellCountY, float roomCellSizeX, float roomCellSizeY);
};


#endif
