#include "LevelGenerator.hpp"



void LevelGenerator::roomHallwayCarve(Room& room, sf::Vector2i side, uint16_t size) {

	const sf::Vector2u roomSizeHalf = room.gridGetSize() / 2u;

	uint16_t xStart = side.x > 0 ? roomSizeHalf.x - size : 1;
	uint16_t xEnd = side.x < 0 ? roomSizeHalf.x + size : room.gridGetSizeX()-1;

	uint16_t yStart = side.y > 0 ? roomSizeHalf.y - size : 1;
	uint16_t yEnd = side.y < 0 ? roomSizeHalf.y + size : room.gridGetSizeY()-1;

	for (uint16_t x = xStart; x < xEnd; x++) {
		for (uint16_t y = yStart; y < yEnd; y++) {

			if ((side.y != 0 && !(x < roomSizeHalf.x - size || x > roomSizeHalf.x + size)) || (side.x != 0 && !(y < roomSizeHalf.y - size || y > roomSizeHalf.y + size))) {
				room.cellGet(x, y).isSolid = false;
			}
		}
	}
}
void LevelGenerator::roomDoorCarve(Room& room, sf::Vector2i side, uint16_t size) {
	
	const sf::Vector2u roomSizeHalf = room.gridGetSize() / 2u;
	
	uint16_t xStart = side.x > 0 ? room.gridGetSizeX() - 1 : 0;
	uint16_t xEnd = side.x < 0 ? 1 : room.gridGetSizeX();

	uint16_t yStart = side.y > 0 ? room.gridGetSizeY() - 1 : 0;
	uint16_t yEnd = side.y < 0 ? 1 : room.gridGetSizeY();

	for (uint16_t x = xStart; x < xEnd; x++) {
		for (uint16_t y = yStart; y < yEnd; y++) {

			if (!((x < roomSizeHalf.x - size || x > roomSizeHalf.x + size) && (y < roomSizeHalf.y - size || y > roomSizeHalf.y + size))) {
				room.cellGet(x, y).isSolid = false;
			}
		}
	}
}
void LevelGenerator::roomSolidify(Room& room) {
	for (uint16_t x = 0; x < room.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < room.gridGetSizeY(); y++) {
			room.cellGet(x, y).isSolid = true;
		}
	}
}
void LevelGenerator::roomSideSolidify(Room& room, sf::Vector2i side) {

	uint16_t xStart = side.x > 0 ? room.gridGetSizeX()-1 : 0;
	uint16_t xEnd = side.x < 0 ? 1 : room.gridGetSizeX();

	uint16_t yStart = side.y > 0 ? room.gridGetSizeY()-1 : 0;
	uint16_t yEnd = side.y < 0 ? 1 : room.gridGetSizeY();

	for (uint16_t x = xStart; x < xEnd; x++) {
		for (uint16_t y = yStart; y < yEnd; y++) {
			room.cellGet(x, y).isSolid = true;
		}
	}
}

void LevelGenerator::roomGridGenerate(RoomGrid& roomGrid) {
	for (uint16_t x = 0; x < roomGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < roomGrid.gridGetSizeY(); y++) {
			roomGenerate(roomGrid.cellGet(x, y));
		}
	}
	ConsoleHandler::consolePrintLoadingGame("RoomGrid generation finished");
}
void LevelGenerator::roomGridConnectionsGenerate(RoomGrid& roomGrid) {
	for (uint16_t x = 0; x < roomGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < roomGrid.gridGetSizeY(); y++) {
			roomConnectionsGenerate(roomGrid.cellGet(x, y), roomGrid);
		}
	}
	ConsoleHandler::consolePrintLoadingGame("RoomGrid connections generation finished");
}
void LevelGenerator::roomGenerate(Room& room) {

	roomSolidify(room);

	roomHallwayCarve(room, sf::Vector2i(1, 0), 4);
	roomHallwayCarve(room, sf::Vector2i(-1, 0), 4);
	roomHallwayCarve(room, sf::Vector2i(0, 1), 4);
	roomHallwayCarve(room, sf::Vector2i(0, -1), 4);

	roomDoorCarve(room, sf::Vector2i(1, 0), 2);
	roomDoorCarve(room, sf::Vector2i(-1, 0), 2);
	roomDoorCarve(room, sf::Vector2i(0, 1), 2);
	roomDoorCarve(room, sf::Vector2i(0, -1), 2);

}
void LevelGenerator::roomConnectionsGenerate(Room& room, RoomGrid& roomGrid) {
	if (room.positionGrid.x > 0) {
		room.connectionLeft = room.positionGrid + RoomGridPosition(-1, 0);
	}
	if (room.positionGrid.y > 0) {
		room.connectionTop = room.positionGrid + RoomGridPosition(0, -1);
	}
	if (room.positionGrid.x < roomGrid.gridGetSizeX() - 1) {
		room.connectionRight = room.positionGrid + RoomGridPosition(1, 0);
	}
	if (room.positionGrid.y < roomGrid.gridGetSizeY() - 1) {
		room.connectionBottom = room.positionGrid + RoomGridPosition(0, 1);
	}
}