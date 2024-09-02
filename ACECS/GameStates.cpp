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

	for (uint32_t x = 320; x < 640; x++) {
		for (uint32_t y = 0; y < 360; y++) {
			//WorldDistortionGrid::pixelSetDistortion(x, y, WorldDistortion(uint16_t(3)));
		}
	}

	EntityManager::entityCreate(0, 0, 0, "Player");
}

void GameStatePause::gameStateUpdate() {
}



