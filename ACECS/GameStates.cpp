#include "GameStates.hpp"

#include "../Include/Common/Math.hpp"
#include "../Include/Game/World/Distortions/WorldDistortionGrid.hpp"
#include "../Include/Game/World/Image Grid/WorldImageGrid.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Graphics.hpp"
#include "Panels.hpp"
#include "World/LevelUpdater.hpp"
#include "../Include/Common/NumberGenerator.hpp"

void GameStatePlay::gameStateUpdate() {

	if (!firstUpdate) {
		gameStateStart();
		firstUpdate = true;
	}
	auto& worldImage = GameLevelGrid::levelGet(0, 0, 0)->imageGrid.cellGet(0, 0);

	worldImage.display();
	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {

	levelGenerate();

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);
	
	sf::Vector2u roomSize = GameLevelGrid::levelGet(0, 0, 0)->levelSize;

	auto& worldImage = GameLevelGrid::levelGet(0, 0, 0)->imageGrid.cellGet(0, 0);

	sf::Vector2f cellSize = gameLevel->roomGrid.cellGet(0, 0).cellsGetSize();

	sf::RectangleShape rectShape;
	rectShape.setSize(cellSize);

	Distortion wallDistortion = Distortion([](sf::Vector2f& heading, sf::Vector2f&) {
		heading *= 0.f;
		}, Cooldown(INFINITY));

	for (uint32_t roomX = 0; roomX < gameLevel->roomGrid.gridGetSizeX(); roomX++) {
		for (uint32_t roomY = 0; roomY < gameLevel->roomGrid.gridGetSizeY(); roomY++) {

			Room& roomCur = gameLevel->roomGrid.cellGet(roomX, roomY);

			for (uint32_t cellX = 0; cellX < roomCur.gridGetSizeX(); cellX++) {
				for (uint32_t cellY = 0; cellY < roomCur.gridGetSizeY(); cellY++) {


					RoomCell& cellCur = roomCur.cellGet(cellX, cellY);

					if (cellCur.isSolid) {
						for (float x = 0; x <= 16; x++) {
							for (float y = 0; y <= 16; y++) {
								gameLevel->distortionGrid.cellSetFromWorld(cellCur.worldPos.position + sf::Vector2f(x, y), WorldDistortion(wallDistortion));
							}
						}
					}

					rectShape.setPosition(cellCur.worldPos.position);
					rectShape.setFillColor(cellCur.color);

					worldImage.draw(rectShape);
				}
			}
		}
	}
	//worldImage.display();

	//Distortion distortion = Distortion([](sf::Vector2f& heading, sf::Vector2f&) {
	//	heading *= 0.99f;
	//	}, Cooldown(INFINITY));

	//for (uint32_t x = 320; x < 1280; x++) {
	//	for (uint32_t y = 0; y < 720; y++) {


	//		gameLevel->distortionGrid.cellSet(x, y, WorldDistortion(distortion));
	//	}
	//}

	EntityManager::entityCreate(0, 0, 0, "Player");
	EntityManager::entityCreate(0, 0, 0, "Skipper");

	EntityId spriteId = EntityManager::entityCreate(0, 0, 0, "Static Sprite");
	Entity& sprite = EntityManager::entityGet(spriteId);
	sprite.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(150, 150);
}
void GameStatePlay::levelGenerate() {

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	gameLevel->roomGridGenerate();
}
void GameStatePause::gameStateUpdate() {
}



