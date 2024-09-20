#include "GameLevel.hpp"

// this file is used for defining functions of the GameLevel class

#include "ECSRegistry.hpp"

GameLevel::GameLevel() :
	levelSize(sf::Vector2u(10000, 10000)),
	imageGrid(WorldImageGrid(1, 1, float(levelSize.x), float(levelSize.y))),
	distortionGrid(WorldDistortionGrid(levelSize.x, levelSize.y))
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
