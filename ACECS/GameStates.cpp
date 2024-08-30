#include "GameStates.hpp"

#include "../Include/Common/Math.hpp"
#include "../Include/Simulation/Distortions/WorldDistortionGrid.hpp"
#include "../Include/Simulation/Image Grid/WorldImageGrid.hpp"
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
	
	auto& worldImage = WorldImageGrid::worldImageFromPixel(0, 0);
	
	uint32_t screenSizeX = 640;
	uint32_t screenSizeY = 360;

	for (uint32_t x = 0; x < screenSizeX; x++) {
		for (uint32_t y = 0; y < screenSizeY; y++) {

			sf::Color chunkColor;

			if (x <= screenSizeX / 2.f && y <= screenSizeY / 2.f) {
				chunkColor = sf::Color::Red;
			}
			if (x > screenSizeX / 2.f && y <= screenSizeY / 2.f) {
				chunkColor = sf::Color::Blue;
			}
			if (x <= screenSizeX / 2.f && y > screenSizeY / 2.f) {
				chunkColor = sf::Color::Green;
			}
			if (x > screenSizeX / 2.f && y > screenSizeY / 2.f) {
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
			WorldDistortionGrid::pixelSetDistortion(x, y, WorldDistortion(uint16_t(3)));
		}
	}

	EntityManager::entityCreate(0, 0, 0, "Player");
}

void GameStatePause::gameStateUpdate() {
}



