#include "../Include/Game/GameData.hpp"
#include "../Include/Game/World/Distortions/DistortionGrid.hpp"
#include "../Include/Game/World/Distortions/DistortionRegistry.hpp"
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
	if (InputInterface::inputGetActive("Portal Create")) {

		using namespace Distortions;

		sf::Vector2f posOffset(-720.f, 0);

		sf::Vector2u portalSideA = levelActive->distortionGrid.coordinatesWorldToCell(mousePos);
		sf::Vector2u portalSideB = levelActive->distortionGrid.coordinatesWorldToCell(mousePos + posOffset);

		constexpr uint32_t cellSize = DistortionGrid::distortionCellSize;

		float offset = 128 / cellSize;
		float halfOffset = offset / 2.f;

		DistortionSharedPtr distortionPortalA = DistortionSharedPtr(new DistortionPositionOffset(posOffset));
		DistortionSharedPtr distortionPortalB = DistortionSharedPtr(new DistortionPositionOffset(-posOffset));

		for (int32_t xOffset = -0; xOffset <= 0; xOffset++) {
			for (int32_t yOffset = -halfOffset; yOffset <= halfOffset; yOffset++) {

				if (Vector2fMath::lengthSqrd(xOffset, yOffset) > halfOffset * halfOffset) continue;

				sf::Vector2u cellPosOffsetA = portalSideA + sf::Vector2u(xOffset, yOffset) + sf::Vector2u(Vector2fMath::normalize(posOffset));
				sf::Vector2u cellPosOffsetB = portalSideB + sf::Vector2u(xOffset, yOffset);

				levelActive->distortionGrid.cellGet(cellPosOffsetA).distortionAdd<DistortionPositionOffset>(distortionPortalA, 120000);
				levelActive->distortionGrid.cellGet(cellPosOffsetB).distortionAdd<DistortionPositionOffset>(distortionPortalB, 120000);
			}
		}
	}

	if (InputInterface::inputGetActive("Slow Field Create")) {

		using namespace Distortions;

		sf::Vector2u slowFieldCenter = levelActive->distortionGrid.coordinatesWorldToCell(mousePos);

		constexpr uint32_t cellSize = DistortionGrid::distortionCellSize;

		float offset = 128 / cellSize;
		float halfOffset = offset / 2.f;

		DistortionSharedPtr distortionSlow = DistortionSharedPtr(new DistortionHeadingMultiply(0.9999f));

		for (int32_t xOffset = -0; xOffset <= 0; xOffset++) {
			for (int32_t yOffset = -halfOffset; yOffset <= halfOffset; yOffset++) {

				if (Vector2fMath::lengthSqrd(xOffset, yOffset) > halfOffset * halfOffset) continue;

				sf::Vector2u cellPosOffset = slowFieldCenter + sf::Vector2u(xOffset, yOffset);

				levelActive->distortionGrid.cellGet(cellPosOffset).distortionAdd<DistortionHeadingMultiply>(distortionSlow, 10000);
			}
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

	level->pathsGenerate();

	level->aStarGrid.cellsAllUpdateNeighbors(level->distortionGrid);

	sf::Vector2u levelSize = level->levelSize;


	// create player and assign the level's playerId to the id of the newly created player
	GameData::playerId = EntityManager::entityCreate(level->levelPosition, "Player");
	Entity& player = EntityManager::entityGet(GameData::playerId);
	player.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000, 2000);

	float offset = 64;
	for (uint16_t i = 0; i < 3; i++) {
		Entity& squadMember = EntityManager::entityGet(EntityManager::entityCreate(level->levelPosition, "Squad Member"));
		squadMember.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - (i * offset), 2000);
	}

	//EntityId spriteAId = EntityManager::entityCreate(level->levelPosition, "Sprite Dynamic");
	//Entity& spriteA = EntityManager::entityGet(spriteAId);
	//spriteA.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - 256, 2000);

	//EntityId spriteBId = EntityManager::entityCreate(level->levelPosition, "Sprite Dynamic");
	//Entity& spriteB = EntityManager::entityGet(spriteBId);
	//spriteB.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - 256, 2000 + 128);

	//EntityId targetPositionId = EntityManager::entityCreate(level->levelPosition, "Sprite Dynamic");
	//Entity& targetPosition = EntityManager::entityGet(targetPositionId);
	//targetPosition.entityComponentGet<EntityComponents::ComponentPosition>()->position = sf::Vector2f(2000 - 500, 2000);
	//targetPosition.entityComponentAdd<EntityComponents::ComponentWinOnPlayerNear>(new EntityComponents::ComponentWinOnPlayerNear);
	//targetPosition.entityComponentAdd<EntityComponents::ComponentEventOnObjectNear>(new EntityComponents::ComponentEventOnObjectNear(32));


	level->grassDraw();
	level->pathsDraw();
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
