#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include <Graphics.hpp>

uint32_t MAX_ENTITIES = 100;
uint16_t MAX_COMPONENT_TYPES = 9;
uint16_t MAX_EVENT_TYPES = 3;

void ECSRegistry::ECSInitialize() {
	EntityManager::entityIdsInitialize();
	EntityComponents::componentIDsInitialize();
	EntityComponents::componentTemplatesInitialize();
	EntityEvents::eventIDsInitialize();
	EventPool::eventPoolInitialize();
}
void ECSRegistry::ECSTerminate() {
	EntityEvents::allEventsTerminate();
	EntityManager::entitiesAllDelete();
}

#pragma region Events

// registers all events
// registering an event gives it an ID which dictates it's update order, lower ID, sooner update.
void EntityEvents::eventIDsInitialize() {

	using EventRegistry = TypeIDAllocator<Event>;

	EventRegistry::typeRegister<EventIDs<EventMove>>();
	EventRegistry::typeRegister<EventIDs<EventRotate>>();
	EventRegistry::typeRegister<EventIDs<EventMoved>>();
}

#pragma endregion Events
#pragma region Components

// registers all components,
// registering a component gives it an ID which dictates it's update order, lower ID, sooner update.
void EntityComponents::componentIDsInitialize() {

	using ComponentRegistry = TypeIDAllocator<Component>;

	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByInput>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotateToMouse>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentDistortionRadius>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSprite>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentViewFollow>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentVisionDrawer>>();
}

#pragma endregion Components
#pragma region Component Templates

void EntityComponents::componentTemplatesInitialize() {
	using namespace EntityComponents;

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Transform",
		/// list of components in template
		{
			createComponentPairFromType<ComponentPosition>(),
			createComponentPairFromType<ComponentRotation>(),
		}
		);

	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Input Controlled",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentMoveByInput>(),
			createComponentPairFromType<ComponentRotateToMouse>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Player",
		{
			"Input Controlled",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentMoveByInput>(120.f),
			createComponentPairFromType<ComponentPosition>(sf::Vector2f(256.f, 256.f)),
			createComponentPairFromType<ComponentRotateToMouse>(0.2f),
			//createComponentPairFromType<ComponentSprite>("Art/Character.png"),
			createComponentPairFromType<ComponentVisionDrawer>(VisionCaster(sf::Vector2f(256.f, 256.f))),
			createComponentPairFromType<ComponentViewFollow>(PanelName::GameView),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Static Sprite",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentSprite>("Art/Test Image 2.png"),
		}
	);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Skipper",
		{
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentPosition>(WorldPosition(0, 0, 0, 64.f, 64.f)),
			createComponentPairFromType<ComponentDistortionRadius>(16.f, Distortion([](sf::Vector2f& heading, sf::Vector2f& position) {
			position += heading * 17.f;
				}, Cooldown(0.1f))),
		}
		);

}

#pragma endregion Component Templates
#pragma region Systems

using namespace EntityComponents;
using namespace EntityEvents;

// if you need to include a certain file for a system, include it here.
#include "../Include/Common/VectorMath.hpp"
#include "../Include/Common/TimeHandler.hpp"
#include "../Include/Game/World/Image Grid/WorldImageGrid.hpp"
#include "../Include/Game/World/Distortions/WorldDistortionGrid.hpp"
#include <iostream>
#include <Input.hpp>
#include "Panels.hpp"
#include <Graphics/WindowHolder.hpp>
#include <Auxiliary/ConsoleHandler.hpp>
#include <numeric>


// if the system is not using the entity parameter, remove it's name to avoid a C4100 error

void ComponentMoveByInput::system(Entity& entity) {
	sf::Vector2f inputAxis;
	inputAxis.x = float(InputInterface::inputGetActive("Move Right") - InputInterface::inputGetActive("Move Left"));
	inputAxis.y = float(InputInterface::inputGetActive("Move Down") - InputInterface::inputGetActive("Move Up"));

	if (inputAxis.x != 0 || inputAxis.y != 0) {
		
		inputAxis = Vector2fMath::normalize(inputAxis) * moveSpeed * static_cast<float>(TimeHandler::deltaSimulatedGet());
		
		auto* moveEvent = entity.entityEventAddAndGet<EventMove>();
		moveEvent->moveAxis = inputAxis;
	}
}
void ComponentRotateToMouse::system(Entity& entity) {
	if (entity.entityComponentHas<ComponentPosition>() && entity.entityComponentHas<ComponentRotation>()) {

		float delta = float(TimeHandler::deltaSimulatedGet());

		auto& gameViewPanel = PanelManager::panelGet(PanelName::GameView);

		float angle = Vector2fMath::angle(entity.entityComponentGet<ComponentPosition>()->position, gameViewPanel.viewMousePositionGet());

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

		float angleDiff = angle - rotationComponent->rotation;

		float rotateAngle = angleDiff;
		// wrap rotation between -PI and +PI
		if (rotateAngle > +Mathf::PI) rotateAngle -= Mathf::TAU;
		if (rotateAngle < -Mathf::PI) rotateAngle += Mathf::TAU;

		// limit the speed of the rotation;
		float rotLimit = Mathf::TAU * 2.f / delta;
		rotateAngle *= lerpSpeed / delta;
		if (rotateAngle > +rotLimit) rotateAngle = +rotLimit;
		if (rotateAngle < -rotLimit) rotateAngle = -rotLimit;

		auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();

		rotateEvent->rotateAmount = rotateAngle * delta;
	}
}
void ComponentPosition::system(Entity& entity) {
	if (entity.entityEventHas<EventMove>()) {
		auto* movedEvent = entity.entityEventAddAndGet<EventMoved>();

		auto moveEvents = entity.entityEventGetAllOfType<EventMove>();
	
		sf::Vector2f heading;

		for (uint32_t i = 0; i < moveEvents.size(); i++) {
			heading += moveEvents[i]->moveAxis;
		}

		sf::Vector2f positionPrev = position;

		// position moved by the heading without applying distortions
		sf::Vector2f positionNatural = position + heading;
		// amount the ray would have moved naturally
		movedEvent->naturalMovedAxis = heading;

		GameLevelGrid::levelGet(worldPosition.level)->distortionGrid.cellGetFromWorld(position).headingApplyDistortion(heading, position);
		
		position += heading;

		// amount the position has unnaturally changed (I.E. by distortions)
		sf::Vector2f axisUnnatural = Vector2fMath::axis(positionNatural, position);
		// amount the ray moved unnaturally
		movedEvent->unnaturalAxis = axisUnnatural;

		// amount the ray moved by the heading
		movedEvent->movedAxis = Vector2fMath::axis(positionPrev, position);
	}
}
void ComponentRotation::system(Entity& entity) {
	if (entity.entityEventHas<EventRotate>()) {

		auto rotateEvents = entity.entityEventGetAllOfType<EventRotate>();

		for (uint32_t i = 0; i < rotateEvents.size(); i++) {
			rotation += rotateEvents[i]->rotateAmount;
		}
	}
}
void ComponentSprite::system(Entity& entity) {
	
	if (!entity.entityComponentHas<ComponentPosition>()) return;
	
	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	sf::Sprite sprite(texture);

	sprite.setPosition(positionComponent->position);

	auto& worldImage = GameLevelGrid::levelGet(positionComponent->worldPosition.level)->imageGrid.cellGetFromWorld(positionComponent->position.x, positionComponent->position.y);

	if (entity.entityComponentHas<ComponentRotation>()) {
		sprite.setRotation(entity.entityComponentGet<ComponentRotation>()->rotation);
	}

	worldImage.draw(sprite);
}
void ComponentVisionDrawer::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentRotation>() && entity.entityComponentHas<ComponentPosition>()) {

		auto& gameViewPanel = PanelManager::panelGet(PanelName::GameView);

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		visionCaster.update(positionComponent->position.x, positionComponent->position.y, rotationComponent->rotation - (Mathf::TAU / 12.f), Mathf::TAU / 6.f, 512);
		visionCaster.memoryUpdate()

		sf::Sprite memorySprite;
		memorySprite.setTexture(visionCaster.renderTextureGet().getTexture());

		gameViewPanel.objectDraw(memorySprite);
		
		sf::Sprite visionSprite;
		visionSprite.setTexture(visionCaster.visionTextureGet().getTexture());
		visionSprite.setPosition(gameViewPanel.viewRect.getPosition());

		gameViewPanel.objectDraw(visionSprite);

	}
}
void ComponentViewFollow::system(Entity& entity) {
	
	if (entity.entityComponentHas<ComponentPosition>()) {
		
		
		auto& panel = PanelManager::panelGet(panelViewToFollow);
		
		if (entity.entityEventHas<EventMoved>()) {
			panel.viewMove(entity.entityEventGet<EventMoved>()->unnaturalAxis);
		}

		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		sf::Vector2f posDiff = positionComponent->position - panel.viewGet().getCenter();

		float lerp = 0.15f;

		panel.viewMove(posDiff * lerp);

		sf::Vector2f mousePos = panel.viewMousePositionGet();

		sf::Vector2f mouseDiff = mousePos - panel.viewGet().getCenter();

		lerp = 0.05f;

		mouseDiff.x *= lerp;
		mouseDiff.y *= lerp * panel.viewAspectRatioGet();

		panel.viewMove(mouseDiff);
	}
}
void ComponentDistortionRadius::system(Entity& entity) {
	if (!entity.entityComponentHas<ComponentPosition>()) return;

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	auto& distortionGrid = GameLevelGrid::levelGet(positionComponent->worldPosition.level)->distortionGrid;

	for (float offsetX = -distortionRadius / 2.f; offsetX <= +distortionRadius / 2.f; offsetX += 1.f) {
		for (float offsetY = -distortionRadius / 2.f; offsetY <= +distortionRadius / 2.f; offsetY += 1.f) {

			if (Vector2fMath::lengthSqrd(offsetX, offsetY) > (distortionRadius * distortionRadius) / (2.f * 2.f)) continue;

			distortionGrid.cellGetFromWorld(positionComponent->position.x + offsetX, positionComponent->position.y + offsetY).distortionAdd(distortion);
		}
	}

	positionComponent->position.x += 30.f * TimeHandler::deltaSimulatedGet();
	positionComponent->position.y += 30.f * TimeHandler::deltaSimulatedGet();

}

#pragma endregion Systems

