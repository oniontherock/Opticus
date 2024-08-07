#include "GameLevel.hpp"

// this file is used for defining functions of the GameLevel class

#include "ECSRegistry.hpp"

GameLevel::GameLevel() {
	entities = std::vector<EntityId>();

	levelSize = sf::Vector2i(1280, 720);
}

GameLevel::GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ) :
	GameLevel()
{
	levelPosition = LevelPosition(_idX, _idY, _idZ);
}

GameLevel::GameLevel(LevelPosition _id) :
	GameLevel(_id.x, _id.y, _id.z)
{}
