#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include <Graphics.hpp>

uint32_t MAX_ENTITIES = 100;
uint16_t MAX_COMPONENT_TYPES = 9;
uint16_t MAX_EVENT_TYPES = 4;

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
	EventRegistry::typeRegister<EventIDs<EventViewMoved>>();
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
			createComponentPairFromType<ComponentRotateToMouse>(0.99f),
			createComponentPairFromType<ComponentVisionDrawer>(VisionCaster(sf::Vector2f(256.f, 256.f)), MemoryHolderVision(sf::Vector2f(640*4, 360*4))),
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

}

#pragma endregion Component Templates
#pragma region Systems

using namespace EntityComponents;
using namespace EntityEvents;

// if you need to include a certain file for a system, include it here.
#include "../Include/Common/VectorMath.hpp"
#include "../Include/Common/TimeHandler.hpp"
#include "../Include/Common/NumberGenerator.hpp"
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

		float deltaLerp = 1.f - powf(1.f - lerpSpeed, delta);

		auto& gameViewPanel = PanelManager::panelGet(PanelName::GameView);

		float angle = Vector2fMath::angle(entity.entityComponentGet<ComponentPosition>()->position, gameViewPanel.viewMousePositionGet());

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

		// wrap rotation between -PI and +PI
		if (rotationComponent->rotation >= +Mathf::PI) rotationComponent->rotation -= Mathf::TAU;
		if (rotationComponent->rotation <= -Mathf::PI) rotationComponent->rotation += Mathf::TAU;
		
		float angleDiff = angle - rotationComponent->rotation;

		float rotateAngle = angleDiff;
		// wrap rotateAngle between -PI and +PI
		if (rotateAngle >= +Mathf::PI) rotateAngle -= Mathf::TAU;
		if (rotateAngle <= -Mathf::PI) rotateAngle += Mathf::TAU;


		// limit the speed of the rotation;
		float rotLimit = Mathf::TAU;
		rotateAngle *= deltaLerp;
		if (rotateAngle > +rotLimit) rotateAngle = +rotLimit;
		if (rotateAngle < -rotLimit) rotateAngle = -rotLimit;

		auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();

		rotateEvent->rotateAmount = rotateAngle;
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

		// amount the camera has moved this frame
		sf::Vector2f cameraMovedAmount;
		if (entity.entityEventHas<EventViewMoved>()) {
			cameraMovedAmount = entity.entityEventGet<EventViewMoved>()->naturalMovedAxis;
		}

		memoryHolder.memoryUpdate(-cameraMovedAmount, visionCaster.visionTextureGet().getTexture());

		sf::Sprite memorySprite;
		memorySprite.setTexture(memoryHolder.memoryGet().getTexture());
		memorySprite.setOrigin(memoryHolder.memorySize / 2.f);
		memorySprite.setPosition(gameViewPanel.viewGet().getCenter());

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
		
		// camera position prior to movement
		sf::Vector2f cameraPositionPrev = panel.viewRect.getPosition();
		// camera position prior to movement,
		sf::Vector2f cameraPositionPrevNaturalized = cameraPositionPrev;

		// has the entity moved?
		if (entity.entityEventHas<EventMoved>()) {
			// if yes, move the camera by the unnatural axis of movement,
			// making a seamless and smooth movement when the entity is traveling through distortions.
			// if this wasn't done, then the camera would quickly launch towards the entity if they travel through a portal, or other distortions,
			// giving away the effect
			panel.viewMove(entity.entityEventGet<EventMoved>()->unnaturalAxis);
			panel.viewUpdate();

			// move the naturalized camera position by the unnatural axis to naturalize it.
			// 
			// this works because:
			// 
			// say the entity moves 500 pixels to the right,
			// 5 of those pixels were a natural movement of the entity, but the other 495 were caused by a teleportation distortion.
			// now if we get the amount the camera moved, by doing "posCur - posPrev" it will say the camera moved by 500 pixels to the right.
			// but if we get it by doing "posCur - (posPrev + unnaturalAxis)", then we will get a movement of only 5 pixels,
			// because posPrev was moved by 495 pixels, which is obviously the unnatural movement.
			// 
			// and so we get the amount the entity moved naturally
			cameraPositionPrevNaturalized += entity.entityEventGet<EventMoved>()->unnaturalAxis;
		}

		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		sf::Vector2f posDiff = positionComponent->position - panel.viewGet().getCenter();

		float lerp = 0.15f;

		panel.viewMove(posDiff * lerp);
		panel.viewUpdate();

		sf::Vector2f mousePos = panel.viewMousePositionGet();

		sf::Vector2f mouseDiff = mousePos - panel.viewGet().getCenter();

		lerp = 0.05f;

		mouseDiff.x *= lerp;
		mouseDiff.y *= lerp * panel.viewAspectRatioGet();

		panel.viewMove(mouseDiff);
		panel.viewUpdate();

		auto* viewMovedEvent = entity.entityEventAddAndGet<EventViewMoved>();
		// get the amount the camera moved
		viewMovedEvent->movedAxis = Vector2fMath::axis(cameraPositionPrev, panel.viewRect.getPosition());
		// get the amount the camera moved naturally
		viewMovedEvent->naturalMovedAxis = Vector2fMath::axis(cameraPositionPrevNaturalized, panel.viewRect.getPosition());
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
}

#pragma endregion Systems

