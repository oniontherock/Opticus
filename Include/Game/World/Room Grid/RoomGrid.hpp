#ifndef __ROOM_GRID_H__
#define __ROOM_GRID_H__

#include "../../../Common/Grid.hpp"
#include "Rooms/Room.hpp"
#include <cstdint>
#include <World/LevelTypeDefinitions.hpp>

class RoomGrid : Grid<Room> {
public:
	RoomGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
	RoomGrid(uint32_t gridSizeX, uint32_t gridSizeY);
};


#endif
