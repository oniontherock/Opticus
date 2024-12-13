#ifndef __LEVEL_GENERATOR_H__
#define __LEVEL_GENERATOR_H__

#include <Auxiliary/VectorMath.hpp>
#include <Auxiliary/Math.hpp>
#include "../Distortions/Distortion.hpp"
#include "../Distortions/DistortionGrid.hpp"
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
	// creates a portal between roomFrom to roomTo
	void roomToRoomPortalCreate(Room& roomFrom, sf::Vector2i sideFrom, Room& roomTo, sf::Vector2i sideTo, uint16_t size, DistortionGrid& distortionGrid);

public:
	void roomGridGenerate(RoomGrid& roomGrid, DistortionGrid& distortionGrid);
	void roomGridConnectionsGenerate(RoomGrid& roomGrid, DistortionGrid& distortionGrid);
	void roomGenerate(Room& room, RoomGrid& roomGrid, DistortionGrid& distortionGrid);
	void roomConnectionsGenerate(Room& room, RoomGrid& roomGrid, DistortionGrid& distortionGrid);


};


#endif
