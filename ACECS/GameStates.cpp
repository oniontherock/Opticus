#include "GameStates.hpp"

#include "../Include/Common/Math.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Graphics.hpp"
#include "Panels.hpp"
#include "World/LevelUpdater.hpp"
#include "../Include/Simulation/Distortions/WorldDistortionGrid.hpp"
#include "../Include/Simulation/WorldImageGrid.hpp"


void GameStatePlay::gameStateUpdate() {

	if (!firstUpdate) {
		gameStateStart();
		firstUpdate = true;
	}

	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {
	uint32_t screenSizeX = 1280;
	uint32_t screenSizeY = 720;

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

			WorldImageGrid::pixelSetColor(x, y, chunkColor);
		}
	}

	auto& worldImage = WorldImageGrid::worldImageFromPixel(0, 0);

	sf::Image testImage;
	if (!testImage.loadFromFile("Art/Test Image.png")) {
		return;
	}
	
	for (uint32_t x = 0; x < testImage.getSize().x; x++) {
		for (uint32_t y = 0; y < testImage.getSize().y; y++) {
			worldImage.setPixel(x + 640, y + 25, testImage.getPixel(x, y));
		}
	}

	for (uint32_t x = 0; x < 1280; x++) {
		for (uint32_t y = 0; y < 720; y++) {
			WorldDistortionGrid::pixelSetDistortion(x, y, WorldDistortion());
		}
	}

	EntityManager::entityCreate(0, 0, 0, "Player");
}

void GameStatePause::gameStateUpdate() {
}



