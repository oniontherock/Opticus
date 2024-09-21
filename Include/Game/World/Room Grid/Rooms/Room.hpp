#ifndef __ROOM_H__
#define __ROOM_H__

#include "../../../../Common/Grid.hpp"
#include "RoomCell.hpp"
#include <cstdint>

typedef sf::Vector2i RoomGridPosition;

#define INVALID_ROOM_POSITION RoomGridPosition(-1, -1)


class Room : public Grid<RoomCell> {
public:
	Room(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
	Room(uint32_t gridSizeX, uint32_t gridSizeY);
	// sets the world positions of every cell in the grid, should only be used after positionWorld is set
	void cellsSetData();

	WorldPosition positionWorld;

	RoomGridPosition positionGrid;

	RoomGridPosition connectionRight = INVALID_ROOM_POSITION;
	RoomGridPosition connectionTop = INVALID_ROOM_POSITION;
	RoomGridPosition connectionLeft = INVALID_ROOM_POSITION;
	RoomGridPosition connectionBottom = INVALID_ROOM_POSITION;


};


#endif


