#ifndef __ROOM_H__
#define __ROOM_H__

#include "../../../../Common/Grid.hpp"
#include "RoomCell.hpp"
#include <cstdint>

class Room : Grid<RoomCell> {
public:
	Room(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
	Room(uint32_t gridSizeX, uint32_t gridSizeY);
};


#endif


