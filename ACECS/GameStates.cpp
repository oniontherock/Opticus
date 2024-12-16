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
	if (InputInterface::inputGetActive("Cell Distort")) {

		using namespace Distortions;

		sf::Vector2f posOffset(-640.f, 0);

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

				levelActive->distortionGrid.cellGet(cellPosOffsetA).distortionAdd<DistortionPositionOffset>(distortionPortalA, 60000);
				levelActive->distortionGrid.cellGet(cellPosOffsetB).distortionAdd<DistortionPositionOffset>(distortionPortalB, 60000);
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

	levelGenerate();

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



	level->backgroundTexture.create(levelSize.x, levelSize.y);

	sf::VertexArray lines;
	lines.setPrimitiveType(sf::Lines);

	uint32_t lineCount = 5000000;

	for (uint32_t i = 0; i < lineCount; i++) {

		sf::Color color = sf::Color(RNGf::getRange(50, 125), RNGf::getRange(100, 255), RNGf::getRange(0, 25), 85);

		sf::Vertex lineStart;
		lineStart.color = color;
		lineStart.position = sf::Vector2f(RNGf::getRange(0.f, levelSize.x), RNGf::getRange(0, levelSize.y));

		sf::Vertex lineEnd;
		lineEnd.color = color;

		// create vector with random heading
		sf::Vector2f lineEndOffset = sf::Vector2f(cos(RNGf::getFullRange(Mathf::PI)), sin(RNGf::getFullRange(Mathf::PI)));
		// scale length of vector
		lineEndOffset = Vector2fMath::lengthSet(lineEndOffset, RNGf::getRange(3.f, 12.f));

		lineEnd.position = lineStart.position + lineEndOffset;

		lines.append(lineStart);
		lines.append(lineEnd);


		sf::Vector2f axis = lineEnd.position - lineStart.position;
		float angleSide = atan2(axis.y, axis.x) + (Mathf::PI * 0.5f);

		sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(1.f, 2.f));

		sf::Vertex lineStartOffsetted = lineStart;
		lineStartOffsetted.position += offset;

		sf::Vertex lineEndOffsetted = lineEnd;
		lineStartOffsetted.position += offset;

		lines.append(lineStartOffsetted);
		lines.append(lineEndOffsetted);

	}

	level->backgroundTexture.draw(lines);

	level->backgroundTexture.display();
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
