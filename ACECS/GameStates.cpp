#include "GameStates.hpp"

#include "../Include/Common/Math.hpp"
#include "../Include/Game/World/Distortions/WorldDistortionGrid.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Graphics.hpp"
#include "Panels.hpp"
#include "World/LevelUpdater.hpp"
#include "../Include/Common/NumberGenerator.hpp"

void GameStatePlay::gameStateUpdate() {

	static int timeState = 1;
	
	if (InputInterface::inputGetActive("Toggle Timescale")) {

		timeState = (timeState + 1) % 3;
	}

	switch (timeState) {
	case 0:
		TimeHandler::timescale = 0.5f;
		break;
	case 1:
		TimeHandler::timescale = 1.f;
		break;
	case 2:
		TimeHandler::timescale = 5.f;
		break;
	}


	if (!firstUpdate) {
		gameStateStart();
		firstUpdate = true;
	}

	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {

	levelGenerate();

	GameLevel* levelActive = GameLevelGrid::levelGet(0, 0, 0);
	
	sf::Vector2u roomSize = GameLevelGrid::levelGet(0, 0, 0)->levelSize;


	// create player and assign the level's playerId to the id of the newly created player
	levelActive->idPlayer = EntityManager::entityCreate(0, 0, 0, "Player");

	EntityId spriteAId = EntityManager::entityCreate(0, 0, 0, "Sprite");
	Entity& spriteA = EntityManager::entityGet(spriteAId);
	spriteA.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(150, 150);

	EntityId spriteBId = EntityManager::entityCreate(0, 0, 0, "Sprite");
	Entity& spriteB = EntityManager::entityGet(spriteBId);
	spriteB.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(150, 300);
}
void GameStatePlay::levelGenerate() {

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	gameLevel->roomGridGenerate();
}
void GameStatePause::gameStateUpdate() {
}



