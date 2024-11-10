#include "Panels.hpp"

#include "../Include/Common/Math.hpp"
#include "../Include/Common/VectorMath.hpp"
#include "../Include/Game/World/Distortions/WorldDistortionGrid.hpp"
#include "../Include/Debugging/ObjectGridRenderer.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "Input.hpp"

void PanelGameView::panelUpdate() {

	checkModeChange();

	auto* levelActive = GameLevelGrid::levelGet(0, 0, 0);

	if (mode == Normal || mode == ObjectGridRender) {
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
}

void PanelGameView::staticDraw(GameLevel* levelActive) {
	levelActive->worldTextureStatic.clear(sf::Color::Transparent);

	// draw white background
	sf::RectangleShape rectangleBackground;
	rectangleBackground.setSize(sf::Vector2f(levelActive->levelSize));
	rectangleBackground.setFillColor(sf::Color::White);

	levelActive->worldTextureStatic.draw(rectangleBackground);

	for (uint32_t i = 0; i < levelActive->staticSpriteEntityIds.size(); i++) {
		Entity& entityCur = EntityManager::entityGet(levelActive->staticSpriteEntityIds[i]);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();

		levelActive->worldTextureStatic.draw(componentSprite->sprite);
	}

	levelActive->worldTextureStatic.display();
}
void PanelGameView::dynamicDraw(GameLevel* levelActive) {
	levelActive->worldTextureDynamic.clear(sf::Color::Transparent);

	for (uint32_t i = 0; i < levelActive->dynamicSpriteEntityIds.size(); i++) {
		Entity& entityCur = EntityManager::entityGet(levelActive->dynamicSpriteEntityIds[i]);

		auto* componentSprite = entityCur.entityComponentGet<EntityComponents::ComponentSprite>();

		levelActive->worldTextureDynamic.draw(componentSprite->sprite);
	}

	levelActive->worldTextureDynamic.display();
}
void PanelGameView::playerDraw(GameLevel* levelActive) {
	// draw player memory
	Entity& player = EntityManager::entityGet(levelActive->idPlayer);

	auto* playerMemoryHolder = player.entityComponentGet<EntityComponents::ComponentMemoryVision>();

	sf::Sprite memorySprite;
	memorySprite.setTexture(playerMemoryHolder->memoryHolder.memoryGet().getTexture());
	memorySprite.setOrigin(playerMemoryHolder->memoryHolder.memorySize / 2.f);
	memorySprite.setPosition(viewGet().getCenter());

	objectDraw(memorySprite);

	// draw player static vision
	auto* playerVisionCasterStatic = player.entityComponentGet<EntityComponents::ComponentVisionCasterStatic>();

	sf::Sprite visionSpriteStatic;
	visionSpriteStatic.setTexture(playerVisionCasterStatic->visionCaster.visionTextureGet().getTexture());
	visionSpriteStatic.setPosition(viewRect.getPosition());

	objectDraw(visionSpriteStatic);

	// draw player dynamic vision
	auto* playerVisionCasterDynamic = player.entityComponentGet<EntityComponents::ComponentVisionCasterDynamic>();

	sf::Sprite visionSpriteDynamic;
	visionSpriteDynamic.setTexture(playerVisionCasterDynamic->visionCaster.visionTextureGet().getTexture());
	visionSpriteDynamic.setPosition(viewRect.getPosition());

	objectDraw(visionSpriteDynamic);
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
