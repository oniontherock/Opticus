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
	auto& worldImage = GameLevelGrid::levelGet(0, 0, 0)->worldGrid.imageGrid.cellGet(0, 0);

	worldImage.display();
	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);
	
	sf::Vector2u roomSize = GameLevelGrid::levelGet(0, 0, 0)->levelSize;

	auto& worldImage = GameLevelGrid::levelGet(0, 0, 0)->worldGrid.imageGrid.cellGet(0, 0);

	uint16_t tileCountX = 160;
	uint16_t tileCountY = 90;

	sf::Vector2f chunkSize = sf::Vector2f(roomSize.x / tileCountX, roomSize.y / tileCountY);

	sf::RectangleShape rectShape;
	rectShape.setSize(chunkSize);

	for (uint32_t x = 0; x < tileCountX; x++) {
		for (uint32_t y = 0; y < tileCountY; y++) {

			sf::Color chunkColor = sf::Color(RNGu8::getUnder(255), RNGu8::getUnder(255), RNGu8::getUnder(255), 255);

			rectShape.setPosition(x * chunkSize.x, y * chunkSize.y);
			rectShape.setFillColor(chunkColor);

			worldImage.draw(rectShape);
		}
	}

	Distortion distortion = Distortion([](sf::Vector2f& heading, sf::Vector2f&) {
		heading *= 0.99f;
		}, Cooldown(INFINITY));

	for (uint32_t x = 320; x < 1280; x++) {
		for (uint32_t y = 0; y < 720; y++) {


			gameLevel->worldGrid.distortionGrid.cellSet(x, y, WorldDistortion(distortion));
		}
	}

	EntityManager::entityCreate(0, 0, 0, "Player");
	EntityManager::entityCreate(0, 0, 0, "Skipper");

	EntityId spriteId = EntityManager::entityCreate(0, 0, 0, "Static Sprite");
	Entity& sprite = EntityManager::entityGet(spriteId);
	sprite.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(150, 150);


}

void GameStatePause::gameStateUpdate() {
}



