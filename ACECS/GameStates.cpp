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

void GameStatePlay::gameStateUpdate() {

	if (!firstUpdate) {
		gameStateStart();
		firstUpdate = true;
	}

	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);
	
	sf::Vector2u roomSize = GameLevelGrid::levelGet(0, 0, 0)->levelSize;

	auto& worldImage = GameLevelGrid::levelGet(0, 0, 0)->worldGrid.imageGrid.worldImageFromPixel(0, 0);

	for (uint32_t x = 0; x < roomSize.x; x++) {
		for (uint32_t y = 0; y < roomSize.y; y++) {

			sf::Color chunkColor;

			if (x <= roomSize.x / 2.f && y <= roomSize.y / 2.f) {
				chunkColor = sf::Color::Red;
			}
			if (x > roomSize.x / 2.f && y <= roomSize.y / 2.f) {
				chunkColor = sf::Color::Blue;
			}
			if (x <= roomSize.x / 2.f && y > roomSize.y / 2.f) {
				chunkColor = sf::Color::Green;
			}
			if (x > roomSize.x / 2.f && y > roomSize.y / 2.f) {
				chunkColor = sf::Color::Yellow;
			}

			worldImage.setPixel(x, y, chunkColor);
		}
	}

	sf::Image testImage;
	if (!testImage.loadFromFile("Art/Test Image 2.png")) {
		return;
	}
	
	for (uint32_t x = 0; x < testImage.getSize().x; x++) {
		for (uint32_t y = 0; y < testImage.getSize().y; y++) {
			worldImage.setPixel(x + 100, y + 100, testImage.getPixel(x, y));
		}
	}

	Distortion distortion = Distortion([](sf::Vector2f& heading, sf::Vector2f&) {
		heading *= 0.99f;
		}, Cooldown(INFINITY));

	for (uint32_t x = 320; x < 1280; x++) {
		for (uint32_t y = 0; y < 720; y++) {


			gameLevel->worldGrid.distortionGrid.pixelSetDistortion(x, y, WorldDistortion(distortion));
		}
	}

	EntityManager::entityCreate(0, 0, 0, "Player");
	EntityManager::entityCreate(0, 0, 0, "Distortion");
}

void GameStatePause::gameStateUpdate() {
}



