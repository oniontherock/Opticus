#ifndef __ROOM_CELL_H__
#define __ROOM_CELL_H__


#include <cstdint>
#include <SFML/System/Vector2.hpp>
#include <World/LevelTypeDefinitions.hpp>


struct RoomCell {
	sf::Vector2f size;
	WorldPosition worldPos;
};


#endif
