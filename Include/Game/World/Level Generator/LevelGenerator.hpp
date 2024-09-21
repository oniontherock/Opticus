#ifndef __LEVEL_GENERATOR_H__
#define __LEVEL_GENERATOR_H__

#include "../Room Grid/RoomGrid.hpp"
#include "../Room Grid/Rooms/Room.hpp"
#include <Auxiliary/ConsoleHandler.hpp>

class LevelGenerator {
protected:

	// carves a hallway from the center of a room to the specified side using the specified size
	void roomHallwayCarve(Room& room, sf::Vector2i side, uint16_t size);
	// carves a door in the specified side of the room using the specified size
	void roomDoorCarve(Room& room, sf::Vector2i side, uint16_t size);
	// sets every cell in a room to solid
	void roomSolidify(Room& room);
	// solidifies the specified side of a room
	void roomSideSolidify(Room& room, sf::Vector2i side);

public:
	void roomGridGenerate(RoomGrid& roomGrid);
	void roomGridConnectionsGenerate(RoomGrid& roomGrid);
	void roomGenerate(Room& room);
	void roomConnectionsGenerate(Room& room, RoomGrid& roomGrid);


};


#endif
