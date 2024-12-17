#include "../Include/Debugging/AStarPathDrawer.hpp"
#include "../Include/Debugging/ObjectGridRenderer.hpp"
#include "../Include/Game/World/Distortions/DistortionCheckDrawer.hpp"
#include "../Include/Game/World/Distortions/DistortionGrid.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "Input.hpp"
#include "Panels.hpp"
#include <Auxiliary/Math.hpp>
#include <Auxiliary/NumberGenerator.hpp>
#include <Auxiliary/VectorMath.hpp>

void PanelGameView::panelUpdate() {

	checkModeChange();

	auto* levelActive = GameLevelGrid::levelGet(0, 0, 0);

	if (mode == Normal || mode == ObjectGridRender || mode == PathsRender) {
		

		staticDraw(levelActive);
		dynamicDraw(levelActive);
		playerDraw(levelActive);
		hudDraw(levelActive);
	}
	if (mode == ObjectGridRender) {
		ObjectGrid& objectGrid = levelActive->objectGrid;

		static ObjectGridRenderer objectGridRenderer;

		objectGridRenderer.objectGridRender(objectGrid);

		sf::Texture objectGridTexture = objectGridRenderer.objectGridTextureGet();

		sf::Sprite objectGridSprite(objectGridTexture);

		objectGridSprite.setScale(objectGrid.cellsGetSize());

		objectDraw(objectGridSprite);
	}
	if (mode == PathsRender) {
		AStarPathDrawer::cellsInvalidDraw(levelActive->aStarGrid);
		AStarPathDrawer::pathsTexture.display();

		sf::Texture pathsTexture = AStarPathDrawer::pathsTexture.getTexture();

		sf::Sprite pathsSprite(pathsTexture);

		objectDraw(pathsSprite);
		AStarPathDrawer::pathsTextureReset();
	}
}

void PanelGameView::checkModeChange() {

	if (InputInterface::inputGetActive("Toggle ObjectGrid Rendering")) {
		if (mode != ObjectGridRender) {
			mode = ObjectGridRender;
		}
		else {
			mode = Normal;
		}
	}
	if (InputInterface::inputGetActive("Toggle Paths Rendering")) {
		if (mode != PathsRender) {
			mode = PathsRender;
		}
		else {
			mode = Normal;
		}
	}
}

void PanelGameView::staticDraw(GameLevel* levelActive) {
	levelActive->worldTextureStatic.clear(sf::Color::Transparent);

	// draw white background
	sf::RectangleShape rectangleBackground;
	rectangleBackground.setSize(sf::Vector2f(levelActive->levelSize));
	rectangleBackground.setFillColor(sf::Color(10, 75, 0, 255));

	levelActive->worldTextureStatic.draw(rectangleBackground);

	sf::Sprite backgroundSprite(levelActive->backgroundTexture.getTexture());
	levelActive->worldTextureStatic.draw(backgroundSprite);

	for (const EntityId& i : levelActive->staticSpriteEntityIds) {
		Entity& entityCur = EntityManager::entityGet(i);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();

		levelActive->worldTextureStatic.draw(componentSprite->sprite);
	}

	levelActive->worldTextureStatic.display();
}
void PanelGameView::dynamicDraw(GameLevel* levelActive) {
	levelActive->worldTextureDynamic.clear(sf::Color::Transparent);

	for (const EntityId& i : levelActive->dynamicSpriteEntityIds) {
		Entity& entityCur = EntityManager::entityGet(i);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();
		auto* componentPosition = entityCur.entityComponentGet<EntityComponents::ComponentPosition>();

		if (componentSprite->checkDistortions) {
			Distortions::drawCheckDistortions(levelActive->worldTextureDynamic, componentSprite->sprite, WorldPosition(entityCur.levelId, componentPosition->position), 64.f, 64.f);
		}
		else {
			levelActive->worldTextureDynamic.draw(componentSprite->sprite);
		}
	}

	levelActive->worldTextureDynamic.display();
}
void PanelGameView::playerDraw(GameLevel*) {

	// draw player memory
	Entity& player = EntityManager::entityGet(GameData::playerId);

	auto* playerMemoryHolder = player.entityComponentGet<EntityComponents::ComponentMemoryVision>();

	sf::Sprite memorySprite;
	memorySprite.setTexture(playerMemoryHolder->memoryHolder.memoryGet().getTexture());
	memorySprite.setOrigin(playerMemoryHolder->memoryHolder.memorySize / 2.f);
	memorySprite.setPosition(viewGet().getCenter());

	objectDraw(memorySprite);

	// draw player vision
	auto* playerVisionCasterHolder = player.entityComponentGet<EntityComponents::ComponentVisionCasterHolder>();

	sf::Sprite visionSprite;
	visionSprite.setTexture(playerVisionCasterHolder->visionCaster.visionTextureGet().getTexture());
	visionSprite.setPosition(viewRect.getPosition());

	objectDraw(visionSprite);
}
void PanelGameView::hudDraw(GameLevel* levelActive) {

	for (uint16_t i = 0; i < levelActive->entities.size(); i++) {
		
		Entity& entity = EntityManager::entityGet(levelActive->entities[i]);

		if (entity.entityComponentHas<EntityComponents::ComponentOrderTargetingDrawer>()) {

			auto* entityComponentOrderTargetingDrawer = entity.entityComponentGet<EntityComponents::ComponentOrderTargetingDrawer>();

			for (uint16_t j = 0; j < entityComponentOrderTargetingDrawer->highlights.size(); j++) {
				objectDraw(entityComponentOrderTargetingDrawer->highlights[j]);
			}
		}
	}
}

void PanelWinScreen::panelUpdate() {

	checkModeChange();

	backgroundDraw();
	textDraw();
}
void PanelWinScreen::checkModeChange() {
}
void PanelWinScreen::backgroundDraw() {
}
void PanelWinScreen::textDraw() {
	sf::Text text;
	sf::Font font;
	font.loadFromFile("Art/Basic bitmap.ttf");
	text.setFont(font);
	text.setString("You Win!");

	text.setPosition(500, 500);
	objectDraw(text);
}

void PanelLoseScreen::panelUpdate() {

	checkModeChange();

	backgroundDraw();
	textDraw();
}
void PanelLoseScreen::checkModeChange() {
}
void PanelLoseScreen::backgroundDraw() {
}
void PanelLoseScreen::textDraw() {
	sf::Text text;
	sf::Font font;
	font.loadFromFile("Art/Basic bitmap.ttf");
	text.setFont(font);
	text.setString("You Lose!");

	text.setPosition(500, 500);
	objectDraw(text);
}

