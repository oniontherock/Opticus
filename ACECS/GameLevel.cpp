#include "GameLevel.hpp"
#include "ECSRegistry.hpp"

GameLevel::GameLevel() :
	levelSize(sf::Vector2u(4096, 4096)),
	distortionGrid(DistortionGrid(levelSize.x, levelSize.y)),
	objectGrid(levelSize.x / 4, levelSize.y / 4, 4.f, 4.f),
	roomGrid(RoomGrid(8, 8, 32, 32, 16.f, 16.f)),
	levelGenerator(LevelGenerator()),
	aStarGrid(AStarGrid(levelSize.x / 16, levelSize.y / 16, 16.f, 16.f)),
	pathGenerator(PathGenerator())
{
	worldTextureStatic.create(levelSize.x, levelSize.y);
	worldTextureDynamic.create(levelSize.x, levelSize.y);

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

void GameLevel::pathsGenerate() {
	pathGenerator.pathGenerate(sf::Vector2f(256, levelSize.y / 2), sf::Vector2f(levelSize.x - 256, levelSize.y / 2));
}
