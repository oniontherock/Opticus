#include "../Include/Game/GameData.hpp"
#include "../Include/Game/World/Distortions/WorldDistortionGrid.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "GameStates.hpp"
#include "Graphics.hpp"
#include "Panels.hpp"
#include "World/LevelUpdater.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>

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
		TimeHandler::timescale = 10.f;
		break;
	}

	GameLevel* levelActive = GameLevelGrid::levelGet(0, 0, 0);

	if (levelActive->firstRun) {
		gameStateStart();
		levelActive->firstRun = false;
	}

	sf::Vector2f mousePos = PanelManager::panelGet(PanelName::GameView).viewMousePositionGet();
	
	if (InputInterface::inputGetActive("Cell Invalidate")) {
		levelActive->aStarGrid.cellGetFromWorld(mousePos).valid = false;
	}
	if (InputInterface::inputGetActive("Cell Distort")) {
		levelActive->distortionGrid.cellGetFromWorld(mousePos).distortionAdd(Distortion(
			[](sf::Vector2f& heading, sf::Vector2f&) {
				heading *= 0.999f;
			}, Cooldown(25.f)
		));
	}

	worldClockUpdate();

	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {

	levelGenerate();

	GameLevel* levelActive = GameLevelGrid::levelGet(0, 0, 0);

	levelActive->aStarGrid.cellsAllUpdateNeighbors(levelActive->distortionGrid);

	sf::Vector2u roomSize = GameLevelGrid::levelGet(0, 0, 0)->levelSize;


	// create player and assign the level's playerId to the id of the newly created player
	GameData::playerId = EntityManager::entityCreate(0, 0, 0, "Player");
	Entity& player = EntityManager::entityGet(GameData::playerId);
	player.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000, 2000);

	float offset = 64;
	for (uint16_t i = 0; i < 3; i++) {
		Entity& squadMember = EntityManager::entityGet(EntityManager::entityCreate(0, 0, 0, "Squad Member"));
		squadMember.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - (i * offset), 2000);
	}

	EntityId spriteAId = EntityManager::entityCreate(0, 0, 0, "Sprite Dynamic");
	Entity& spriteA = EntityManager::entityGet(spriteAId);
	spriteA.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000-256, 2000);

	EntityId spriteBId = EntityManager::entityCreate(0, 0, 0, "Sprite Dynamic");
	Entity& spriteB = EntityManager::entityGet(spriteBId);
	spriteB.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000-256, 2000+128);

	EntityId targetPositionId = EntityManager::entityCreate(0, 0, 0, "Sprite Dynamic");
	Entity& targetPosition = EntityManager::entityGet(targetPositionId);
	spriteB.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - 500, 2000);
	spriteB.entityComponentAdd<EntityComponents::ComponentWinOnPlayerNear>(new EntityComponents::ComponentWinOnPlayerNear);
	spriteB.entityComponentAdd<EntityComponents::ComponentEventOnObjectNear>(new EntityComponents::ComponentEventOnObjectNear(32));

}
void GameStatePlay::levelGenerate() {

	GameLevel* gameLevel = GameLevelGrid::levelGet(0, 0, 0);

	gameLevel->roomGridGenerate();
}
void GameStatePlay::worldClockUpdate() {
	GameData::worldClock.update(TimeHandler::timeSimulatedGet());

	if (GameData::worldClock.dayCurrentGet() >= GameData::worldClock.dayCountMax) {
		GameStateHandler::gameStateForceSet(GameStateType::Lose);
	}
}

void GameStatePause::gameStateUpdate() {
	WindowHolder::windowGet().close();
}


void GameStateWin::gameStateUpdate() {

}

void GameStateLose::gameStateUpdate() {
	if (exitCooldown.updateAutoReset(TimeHandler::deltaSimulatedGet())) {
		WindowHolder::windowGet().close();
	}
}
