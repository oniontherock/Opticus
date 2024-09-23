#include "GameLevel.hpp"

// this file is used for defining functions of the GameLevel class

#include "ECSRegistry.hpp"

GameLevel::GameLevel() :
	levelSize(sf::Vector2u(2500, 2500)),
	imageGrid(WorldImageGrid(1, 1, float(levelSize.x), float(levelSize.y))),
	distortionGrid(WorldDistortionGrid(levelSize.x, levelSize.y)),
	roomGrid(RoomGrid(4, 4, 32, 32, 16.f, 16.f)),
	levelGenerator(LevelGenerator())
{
	entities = std::vector<EntityId>();
}

GameLevel::GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ) :
	GameLevel()
{
	levelPosition = LevelPosition(_idX, _idY, _idZ);
}

GameLevel::GameLevel(LevelPosition _id) :
	GameLevel(_id.x, _id.y, _id.z)
{}

void GameLevel::roomGridGenerate() {
	levelGenerator.roomGridConnectionsGenerate(roomGrid, distortionGrid);
	levelGenerator.roomGridGenerate(roomGrid, distortionGrid);
}
