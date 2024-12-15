#include "LevelGenerator.hpp"
#include <iostream>


void LevelGenerator::roomHallwayCarve(Room& room, sf::Vector2i side, uint16_t size) {

	const sf::Vector2u roomSizeHalf = room.gridGetSize() / 2u;

	uint16_t xStart = uint16_t(side.x > 0 ? roomSizeHalf.x - size : 1);
	uint16_t xEnd = uint16_t(side.x < 0 ? roomSizeHalf.x + size : room.gridGetSizeX() - 1);

	uint16_t yStart = uint16_t(side.y > 0 ? roomSizeHalf.y - size : 1);
	uint16_t yEnd = uint16_t(side.y < 0 ? roomSizeHalf.y + size : room.gridGetSizeY() - 1);

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
	
	uint16_t xStart = uint16_t(side.x > 0 ? room.gridGetSizeX() - 1 : 0);
	uint16_t xEnd = uint16_t(side.x < 0 ? 1 : room.gridGetSizeX());

	uint16_t yStart = uint16_t(side.y > 0 ? room.gridGetSizeY() - 1 : 0);
	uint16_t yEnd = uint16_t(side.y < 0 ? 1 : room.gridGetSizeY());

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

	uint16_t xStart = uint16_t(side.x > 0 ? room.gridGetSizeX() - 1 : 0);
	uint16_t xEnd = uint16_t(side.x < 0 ? 1 : room.gridGetSizeX());

	uint16_t yStart = uint16_t(side.y > 0 ? room.gridGetSizeY() - 1 : 0);
	uint16_t yEnd = uint16_t(side.y < 0 ? 1 : room.gridGetSizeY());

	for (uint16_t x = xStart; x < xEnd; x++) {
		for (uint16_t y = yStart; y < yEnd; y++) {
			room.cellGet(x, y).isSolid = true;
		}
	}
}

void LevelGenerator::roomToRoomPortalCreate(Room& roomFrom, sf::Vector2i sideFrom, Room& roomTo, sf::Vector2i sideTo, uint16_t, DistortionGrid&) {
	
	const uint16_t xFromStart = uint16_t(sideFrom.x > 0 ? roomFrom.gridGetSizeX() - 1 : 0);
	const uint16_t xFromEnd = uint16_t(sideFrom.x < 0 ? 1 : roomFrom.gridGetSizeX());
	
	const uint16_t yFromStart = uint16_t(sideFrom.y > 0 ? roomFrom.gridGetSizeY() - 1 : 0);
	const uint16_t yFromEnd = uint16_t(sideFrom.y < 0 ? 1 : roomFrom.gridGetSizeY());
	
	const uint16_t xToStart = uint16_t(sideTo.x > 0 ? roomTo.gridGetSizeX() - 1 : 0);
	const uint16_t xToEnd = uint16_t(sideTo.x < 0 ? 1 : roomTo.gridGetSizeX());
	
	const uint16_t yToStart = uint16_t(sideTo.y > 0 ? roomTo.gridGetSizeY() - 1 : 0);
	const uint16_t yToEnd = uint16_t(sideTo.y < 0 ? 1 : roomTo.gridGetSizeY());

	// we get the cell size only from roomFrom because the cell size must be the same for rooms in the same roomGrid
	sf::Vector2f cellSize = roomFrom.cellsGetSize();

	int16_t xFrom = xFromStart - 1;
	int16_t xTo = xToStart - 1;
	while ((xFrom < xFromEnd - 1) && (xTo < xToEnd - 1)) {
		xFrom++;
		xTo++;

		int16_t yFrom = yFromStart - 1;
		int16_t yTo = yToStart - 1;
		while ((yFrom < yFromEnd - 1) && (yTo < yToEnd - 1)) {
			yFrom++;
			yTo++;

			// bool representing whether the sideFrom is on the X or Y axis, true if on the X axis.
			const bool fromAxis = sideFrom.x != 0 ? true : false;
			// bool representing whether the sideTo is on the X or Y axis, true if on the X axis.
			const bool toAxis = sideTo.x != 0 ? true : false;

			//std::vector<Distortion> cellDistortions;

			const sf::Vector2f cellToCenter = roomTo.cellGet(xTo, yTo).worldPos.position + (cellSize / 2.f);

			const sf::Vector2i cellTeleportSide = sf::Vector2i(cellToCenter - (sf::Vector2f(sideTo.x * (cellSize.x / 2.f), sideTo.y * (cellSize.y / 2.f))) / 4.f);
			const sf::Vector2i portalDirection = Vector2iMath::rotate(Vector2iMath::abs(sideTo), Mathf::PI / 2.f);

			// offset for the teleporter to teleport to.
			// this is necessary because we are iterating over cells,
			// and since a cell is almost always larger than 1x1, placing a distortion only on the cell wont work
			const int32_t cellToAxisSize = int32_t((toAxis ? cellSize.x : cellSize.y));
			for (int32_t subCellOffset = 0; subCellOffset < cellToAxisSize; subCellOffset++) {

				const sf::Vector2i portalOffset = portalDirection * (subCellOffset - (cellToAxisSize / 2));

				const sf::Vector2f teleportToLocation = sf::Vector2f(cellTeleportSide + portalOffset - sideTo);

				//cellDistortions.push_back(
				//	Distortion([teleportToLocation](sf::Vector2f& heading, sf::Vector2f& position) {
				//		position = teleportToLocation;
				//		position -= heading;
				//	}, Cooldown(INFINITY)));
			}

			const sf::Vector2f cellFromCenter = roomFrom.cellGet(xFrom, yFrom).worldPos.position + (cellSize / 2.f);

			const sf::Vector2i cellDistortionSide = sf::Vector2i(cellFromCenter - (sf::Vector2f(sideFrom.x * (cellSize.x / 2.f), sideFrom.y * (cellSize.y / 2.f)) / 4.f));
			const sf::Vector2i distortionDirection = Vector2iMath::rotate(Vector2iMath::abs(sideFrom), Mathf::PI / 2.f);

			// offset for the distortionCell to be placed at.
			// this is necessary because we are iterating over cells,
			// and since a cell is almost always larger than 1x1, placing a distortion only on the cell wont work
			const int32_t cellFromAxisSize = int32_t(fromAxis ? cellSize.x : cellSize.y);
			for (int32_t subCellOffset = 0; subCellOffset < cellFromAxisSize; subCellOffset++) {

				const sf::Vector2i distortionOffset = distortionDirection * (subCellOffset - (cellFromAxisSize / 2));

				const sf::Vector2f distortionCellLocation = sf::Vector2f(cellDistortionSide + distortionOffset);

				//distortionGrid.cellGetFromWorld(distortionCellLocation).distortionAdd(cellDistortions[subCellOffset]);
			}
		}
	}
}


void LevelGenerator::roomGridGenerate(RoomGrid& roomGrid, DistortionGrid& distortionGrid) {
	for (uint16_t x = 0; x < roomGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < roomGrid.gridGetSizeY(); y++) {
			roomGenerate(roomGrid.cellGet(x, y), roomGrid, distortionGrid);
		}
	}
	ConsoleHandler::consolePrintLoadingGame("RoomGrid generation finished");
}
void LevelGenerator::roomGridConnectionsGenerate(RoomGrid& roomGrid, DistortionGrid& distortionGrid) {
	for (uint16_t x = 0; x < roomGrid.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < roomGrid.gridGetSizeY(); y++) {
			roomConnectionsGenerate(roomGrid.cellGet(x, y), roomGrid, distortionGrid);
		}
	}
	ConsoleHandler::consolePrintLoadingGame("RoomGrid connections generation finished");
}
void LevelGenerator::roomGenerate(Room& room, RoomGrid& roomGrid, DistortionGrid& distortionGrid) {

	roomSolidify(room);

	if (room.connectionLeft != INVALID_ROOM_POSITION) {
		roomHallwayCarve(room, sf::Vector2i(-1, 0), 4);
		roomDoorCarve(room, sf::Vector2i(-1, 0), 2);
		roomToRoomPortalCreate(room, sf::Vector2i(-1, 0), roomGrid.cellGet(room.connectionLeft.x, room.connectionLeft.y), sf::Vector2i(1, 0), 4, distortionGrid);
	}
	if (room.connectionRight != INVALID_ROOM_POSITION) {
		roomHallwayCarve(room, sf::Vector2i(1, 0), 4);
		roomDoorCarve(room, sf::Vector2i(1, 0), 2);
		roomToRoomPortalCreate(room, sf::Vector2i(1, 0), roomGrid.cellGet(room.connectionRight.x, room.connectionRight.y), sf::Vector2i(-1, 0), 4, distortionGrid);
	}
	if (room.connectionTop != INVALID_ROOM_POSITION) {
		roomHallwayCarve(room, sf::Vector2i(0, -1), 4);
		roomDoorCarve(room, sf::Vector2i(0, -1), 2);
		roomToRoomPortalCreate(room, sf::Vector2i(0, -1), roomGrid.cellGet(room.connectionTop.x, room.connectionTop.y), sf::Vector2i(0, 1), 4, distortionGrid);
	}
	if (room.connectionBottom != INVALID_ROOM_POSITION) {
		roomHallwayCarve(room, sf::Vector2i(0, 1), 4);
		roomDoorCarve(room, sf::Vector2i(0, 1), 2);
		roomToRoomPortalCreate(room, sf::Vector2i(0, 1), roomGrid.cellGet(room.connectionBottom.x, room.connectionBottom.y), sf::Vector2i(0, -1), 4, distortionGrid);
	}

	sf::Color roomColor = sf::Color(sf::Uint8(rand()), sf::Uint8(rand()), sf::Uint8(rand()), 255);

	for (uint16_t x = 0; x < room.gridGetSizeX(); x++) {
		for (uint16_t y = 0; y < room.gridGetSizeY(); y++) {
			room.cellGet(x, y).color = roomColor;
		}
	}
}
void LevelGenerator::roomConnectionsGenerate(Room& room, RoomGrid& roomGrid, DistortionGrid&) {
	if (uint16_t(room.positionGrid.x) < roomGrid.gridGetSizeX() - 1) {
		room.connectionRight = room.positionGrid + RoomGridPosition(1, 0);
	}
	if (uint16_t(room.positionGrid.x) > 0) {
		room.connectionLeft = room.positionGrid + RoomGridPosition(-1, 0);
	}
	if (uint16_t(room.positionGrid.y) > 0) {
		room.connectionTop = room.positionGrid + RoomGridPosition(0, -1);
	}
	if (uint16_t(room.positionGrid.y) < roomGrid.gridGetSizeY() - 1) {
		room.connectionBottom = room.positionGrid + RoomGridPosition(0, 1);
	}
}