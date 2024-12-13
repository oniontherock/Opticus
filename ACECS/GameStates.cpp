#include "../Include/Game/GameData.hpp"
#include "../Include/Game/World/Distortions/DistortionGrid.hpp"
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


	if (firstUpdate) {
		gameStateStart();
		firstUpdate = false;
	}
	if (levelActive->firstRun) {
		levelStart(levelActive);
		levelActive->firstRun = false;
	}

	sf::Vector2f mousePos = PanelManager::panelGet(PanelName::GameView).viewMousePositionGet();
	
	if (InputInterface::inputGetActive("Cell Invalidate")) {
		levelActive->aStarGrid.cellGetFromWorld(mousePos).valid = false;
	}
	if (InputInterface::inputGetActive("Cell Distort")) {

		sf::Vector2u distortionCellPos = levelActive->distortionGrid.coordinatesWorldToCell(mousePos);

		float offset = 64;

		for (int32_t yOffset = -16; yOffset <= 16; yOffset++) {
			levelActive->distortionGrid.cellGet(distortionCellPos + sf::Vector2u(0, yOffset)).distortionAdd(DistortionType::PositionOffset, sf::Vector2f(-offset*4, 0), 50000);
			levelActive->distortionGrid.cellGet(distortionCellPos + sf::Vector2u(-offset+1, yOffset)).distortionAdd(DistortionType::PositionOffset, sf::Vector2f(offset*4, 0), 50000);
		}

	}

	worldClockUpdate();

	EntityManager::entitiesIntangibleUpdate();
	EntityManager::entitiesQueuedUpdate();
	LevelUpdater::levelsUpdate();
}

void GameStatePlay::gameStateStart() {

	sf::Image& img = GraphicsStore::imageStore.fileGetOrLoadFromName("Art/Perlin Noise Seamless", "png");

	sf::Texture tex;
	tex.loadFromImage(img);

	GraphicsStore::textureStore.objectAddFromInstance("Perlin Noise Seamless", tex);
}
void GameStatePlay::levelStart(GameLevel* level) {

	levelGenerate();

	level->aStarGrid.cellsAllUpdateNeighbors(level->distortionGrid);

	sf::Vector2u roomSize = level->levelSize;


	// create player and assign the level's playerId to the id of the newly created player
	GameData::playerId = EntityManager::entityCreate(level->levelPosition, "Player");
	Entity& player = EntityManager::entityGet(GameData::playerId);
	player.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000, 2000);

	float offset = 64;
	for (uint16_t i = 0; i < 3; i++) {
		Entity& squadMember = EntityManager::entityGet(EntityManager::entityCreate(level->levelPosition, "Squad Member"));
		squadMember.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - (i * offset), 2000);
	}

	EntityId spriteAId = EntityManager::entityCreate(level->levelPosition, "Sprite Dynamic");
	Entity& spriteA = EntityManager::entityGet(spriteAId);
	spriteA.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - 256, 2000);

	EntityId spriteBId = EntityManager::entityCreate(level->levelPosition, "Sprite Dynamic");
	Entity& spriteB = EntityManager::entityGet(spriteBId);
	spriteB.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - 256, 2000 + 128);

	EntityId targetPositionId = EntityManager::entityCreate(level->levelPosition, "Sprite Dynamic");
	Entity& targetPosition = EntityManager::entityGet(targetPositionId);
	targetPosition.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - 500, 2000);
	targetPosition.entityComponentAdd<EntityComponents::ComponentWinOnPlayerNear>(new EntityComponents::ComponentWinOnPlayerNear);
	targetPosition.entityComponentAdd<EntityComponents::ComponentEventOnObjectNear>(new EntityComponents::ComponentEventOnObjectNear(32));
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
	if (exitCooldown.updateAutoReset(float(TimeHandler::deltaSimulatedGet()))) {
		WindowHolder::windowGet().close();
	}
}
