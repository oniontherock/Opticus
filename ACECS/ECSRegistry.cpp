#include "../Include/Game/AI/Utility AI/States/AIStates.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include <Graphics.hpp>

uint32_t MAX_ENTITIES = 100;
uint16_t MAX_COMPONENT_TYPES = 21;
uint16_t MAX_EVENT_TYPES = 8;

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
	EventRegistry::typeRegister<EventIDs<EventObjectSeen>>();
	EventRegistry::typeRegister<EventIDs<EventVisionUpdated>>();
	EventRegistry::typeRegister<EventIDs<EventActorTurnTo>>();
	EventRegistry::typeRegister<EventIDs<EventActorGoTo>>();
}

#pragma endregion Events
#pragma region Components

// registers all components,
// registering a component gives it an ID which dictates it's update order, lower ID, sooner update.
void EntityComponents::componentIDsInitialize() {

	using ComponentRegistry = TypeIDAllocator<Component>;

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectTypeAssigner>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteDynamicRegister>>();
	
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorStateTicker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorMovementHandler>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByInput>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotateToMouse>>();
	
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridInhabiterRadius>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentDistortionRadius>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentSprite>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentViewFollow>>();
	
	// senses/memory
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectVision>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectMemory>>();

	// Actor/AI
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorBlackboard>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorData>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorStateManager>>();

	// world vision/memory
	ComponentRegistry::typeRegister<ComponentIDs<ComponentVisionCasterStatic>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentVisionCasterDynamic>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentMemoryVision>>();

	// debug
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectMemoryDebug>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectVisionDebug>>();
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
		"Sprite",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentSpriteDynamicRegister>(),
			createComponentPairFromType<ComponentSprite>("Art/Error texture.png"),
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
		"Actor",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectVision>(0.1f),
			createComponentPairFromType<ComponentObjectMemory>(),
			createComponentPairFromType<ComponentActorBlackboard>([](Entity& actor, ActorBlackboard& actorBlackboard) {
				}),
			createComponentPairFromType<ComponentActorData>(ActorDataHolder(TraitVector{ 50.f, 50, 25, 75}, EmotionVector{ 0 },
				[](const ActorBlackboard& actorBlackboard, ActorDataHolder& actorData) {

					const float delta = TimeHandler::deltaSimulatedGet();
					
					const float courageousness = actorData.traitGet(ActorTrait::Courageousness);

					auto memory = actorBlackboard.dataGet<ObjectMemoryHolder>("Memory");

					if (memory.memoryHasType(uint8_t(ObjectType::Door))) {

						float doorSoonestSeenValue = 9999.f;
						uint16_t doorSoonestSeenInd = UINT16_MAX;

						std::vector<ObjectMemory> memoriesDoors = memory.memoriesGetOfType(ObjectType::Door);

						for (uint16_t i = 0; i < memoriesDoors.size(); i++) {
							if (doorSoonestSeenValue > memoriesDoors[i].second.value) {
								doorSoonestSeenValue = memoriesDoors[i].second.value;
								doorSoonestSeenInd = i;
							}
						}
						// seen door in the last 10 seconds
						if (doorSoonestSeenValue <= 1.f) {
							actorData.emotionIncrement(ActorEmotion::Fear, delta * (101.f - courageousness));
						}
						else {
							actorData.emotionIncrement(ActorEmotion::Fear, ((doorSoonestSeenValue * doorSoonestSeenValue) * (delta * delta)));
						}
					}
							actorData.emotionIncrement(ActorEmotion::Fear, 10.f * delta);
				})
				),
			createComponentPairFromType<ComponentActorStateManager>(std::vector<UtilityStates::StateBase*>{
				new UtilityStates::StateIdle(), 
				new UtilityStates::StateWander()
			}),
			createComponentPairFromType<ComponentActorStateTicker>(),
			createComponentPairFromType<ComponentActorMovementHandler>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Player",
		{
			"Input Controlled",
			"Actor",
			"Sprite",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Player),
			createComponentPairFromType<ComponentMoveByInput>(120.f),
			createComponentPairFromType<ComponentPosition>(sf::Vector2f(256.f, 256.f)),
			createComponentPairFromType<ComponentRotateToMouse>(0.99f),
			createComponentPairFromType<ComponentVisionCasterStatic>(VisionCaster(sf::Vector2f(256.f, 256.f))),
			createComponentPairFromType<ComponentVisionCasterDynamic>(VisionCaster(sf::Vector2f(256.f, 256.f))),
			createComponentPairFromType<ComponentMemoryVision>(MemoryHolderVision(sf::Vector2f(640 * 4, 360 * 4))),
			createComponentPairFromType<ComponentViewFollow>(PanelName::GameView),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(16),
			createComponentPairFromType<ComponentSprite>("Art/Squad Member.png"),
			//createComponentPairFromType<ComponentObjectVisionDebug>(),
			//createComponentPairFromType<ComponentObjectMemoryDebug>(),
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
#include "../Include/Game/World/Objects/ObjectRegistry.hpp"
#include "../Include/Common/NumberGenerator.hpp"
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

	sprite.setTexture(texture);
	sprite.setOrigin(sf::Vector2f(texture.getSize()) / 2.f);
	sprite.setPosition(positionComponent->position);

	auto& worldTextureDynamic = GameLevelGrid::levelGet(positionComponent->worldPosition.level)->worldTextureDynamic;

	if (entity.entityComponentHas<ComponentRotation>()) {
		sprite.setRotation(entity.entityComponentGet<ComponentRotation>()->rotation * 180.f / Mathf::PI);
	}
}
void ComponentVisionCasterStatic::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentRotation>() && entity.entityComponentHas<ComponentPosition>()) {

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		visionCaster.textureToSeeSet(GameLevelGrid::levelGet(positionComponent->worldPosition.level)->worldTextureStatic);
		visionCaster.update(positionComponent->position.x, positionComponent->position.y, rotationComponent->rotation - (Mathf::TAU / 12.f), Mathf::TAU / 6.f, 520.f, 256);

		auto* eventVisionUpdated = entity.entityEventAddAndGet<EventVisionUpdated>();
		eventVisionUpdated->textureToMemorize = visionCaster.visionTextureGet().getTexture();
	}
}
void ComponentVisionCasterDynamic::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentRotation>() && entity.entityComponentHas<ComponentPosition>()) {

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		visionCaster.textureToSeeSet(GameLevelGrid::levelGet(positionComponent->worldPosition.level)->worldTextureDynamic);
		visionCaster.update(positionComponent->position.x, positionComponent->position.y, rotationComponent->rotation - (Mathf::TAU / 12.f), Mathf::TAU / 6.f, 520.f, 256);
	}
}
void ComponentMemoryVision::system(Entity& entity) {

	if (!entity.entityEventHas<EventVisionUpdated>()) return;

	auto eventVisionUpdatedAll = entity.entityEventGetAllOfType<EventVisionUpdated>();
	
	// amount the camera has moved this frame
	sf::Vector2f cameraMovedAmount;
	if (entity.entityEventHas<EventViewMoved>()) {
		cameraMovedAmount = entity.entityEventGet<EventViewMoved>()->naturalMovedAxis;
	}

	for (uint16_t i = 0; i < eventVisionUpdatedAll.size(); i++) {
		memoryHolder.memoryUpdate(-cameraMovedAmount, eventVisionUpdatedAll[i]->textureToMemorize);
		
		cameraMovedAmount = sf::Vector2f(0.f, 0.f);
	}
}
void ComponentObjectVision::system(Entity& entity) {
	if (cooldownVisionUpdate.updateAutoReset(TimeHandler::deltaSimulatedGet())) {
		if (entity.entityComponentHas<ComponentRotation>() && entity.entityComponentHas<ComponentPosition>()) {

			auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
			auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

			objectVision.update(positionComponent->position.x, positionComponent->position.y, rotationComponent->rotation - (Mathf::TAU / 12.f), Mathf::TAU / 6.f, 475.f, 32);

			ObjectIdVector& objectsSeenSet = objectVision.objectsSeenGet();

			if (objectsSeenSet.size() <= 0) return;

			auto* eventObjectSeen = entity.entityEventAddAndGet<EventObjectSeen>();
			eventObjectSeen->objectsSeen = &objectsSeenSet;
		}
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
void ComponentObjectTypeAssigner::system(Entity& entity) {
	ObjectRegistry::entityObjectTypeAssign(entity.Id, objectType);

	entity.entityComponentTerminate<ComponentObjectTypeAssigner>();
}
void ComponentSpriteDynamicRegister::system(Entity& entity) {

	try {
		if (!entity.entityComponentHas<ComponentSprite>()) {
			throw "Does not have ComponentSprite";
		}
		if (!entity.entityComponentHas<ComponentPosition>()) {
			throw "Does not have ComponentPosition";
		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentSpriteDynamicRegister system failed: Exception:");
		return;
	}

	auto* componentPosition = entity.entityComponentGet<ComponentPosition>();

	GameLevelGrid::levelGet(componentPosition->worldPosition.level)->dynamicSpriteEntityIds.push_back(entity.Id);

	entity.entityComponentTerminate<ComponentSpriteDynamicRegister>();
}
void ComponentObjectGridInhabiterRadius::system(Entity& entity) {
	// check that entity has ComponentPosition
	if (!entity.entityComponentHas<ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("ComponentObjectGridInhabiterRadius assigned to an entity without a ComponentPosition!");

		entity.entityComponentTerminate<ComponentObjectGridInhabiterRadius>();
		return;
	}

	// check that entity has ObjectType
	if (ObjectRegistry::entityObjectTypeGet(entity.Id) == ObjectType::Null) {
		ConsoleHandler::consolePrintErr("ComponentObjectGridInhabiterRadius assigned to an entity without an ObjectType!");

		entity.entityComponentTerminate<ComponentObjectGridInhabiterRadius>();
		return;
	}

	auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

	if (positionPrev == WorldPosition(0, 0, 0, 0, 0)) {
		positionPrev = positionComponent->worldPosition;
	}

	// note that two separate objectGrid references are used because an entity could travel to a different level.
	
	auto& objectGridDepopulation = GameLevelGrid::levelGet(positionPrev.level)->objectGrid;

	for (float offsetX = -radius / 2.f; offsetX <= +radius / 2.f; offsetX += 1.f) {
		for (float offsetY = -radius / 2.f; offsetY <= +radius / 2.f; offsetY += 1.f) {

			if (Vector2fMath::lengthSqrd(offsetX, offsetY) > (radius * radius) / (2.f * 2.f)) continue;

			objectGridDepopulation.cellGetFromWorld(positionPrev.position.x + offsetX, positionPrev.position.y + offsetY).idRemove(entity.Id);
		}
	};

	auto& objectGridPopulation = GameLevelGrid::levelGet(positionComponent->worldPosition.level)->objectGrid;

	for (float offsetX = -radius / 2.f; offsetX <= +radius / 2.f; offsetX += 1.f) {
		for (float offsetY = -radius / 2.f; offsetY <= +radius / 2.f; offsetY += 1.f) {

			if (Vector2fMath::lengthSqrd(offsetX, offsetY) > (radius * radius) / (2.f * 2.f)) continue;

			objectGridPopulation.cellGetFromWorld(positionComponent->position.x + offsetX, positionComponent->position.y + offsetY).idAdd(entity.Id);
		}
	};

	positionPrev = positionComponent->worldPosition;
}
void ComponentObjectMemoryDebug::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentObjectMemory>()) {

		cooldownPrint.update(TimeHandler::deltaRealGet());

		if (cooldownPrint.ready()) {

			constexpr const char* objectTypesNames[] = {
			   "Null",
			   "Player",
			   "SquadMember",
			   "Door",
			   "Skipper",
			   "Wall",
			   "SIZE",
			};

			auto* componentObjectMemory = entity.entityComponentGet<ComponentObjectMemory>();

			std::string string;

			cooldownPrint.reset();

			string += "[";
			for (uint16_t i = 0; i < uint16_t(ObjectType::SIZE); i++) {

				std::vector<ObjectMemory>& memoryVector = componentObjectMemory->objectMemoryHolder.memoriesGetOfType(i);
				for (uint16_t j = 0; j < memoryVector.size(); j++) {
					string += "(";
					string += objectTypesNames[i];
					string += ", ";
					string += std::to_string(memoryVector[j].second.value);
					string += "), ";
				}
			}
			string += "]";
			ConsoleHandler::consolePrintColor(string, 3);
		}
	}
}
void ComponentObjectVisionDebug::system(Entity& entity) {

	cooldownPrint.update(TimeHandler::deltaRealGet());

	if (cooldownPrint.ready()) {

		constexpr const char* objectTypesNames[] = {
		   "Null",
		   "Player",
		   "SquadMember",
		   "Door",
		   "Skipper",
		   "Wall",
		   "SIZE",
		};

		std::string string;

		if (entity.entityEventHas<EventObjectSeen>()) {
			cooldownPrint.reset();

			auto* eventObjectSeen = entity.entityEventGet<EventObjectSeen>();

			const ObjectIdVector* objectsSeenVector = eventObjectSeen->objectsSeen;

			string += "[";
			for (uint16_t i = 0; i < objectsSeenVector->size(); i++) {

				for (uint16_t j = 0; j < objectsSeenVector->at(i).size(); j++) {
					string += objectTypesNames[i];
					string += ", ";
				}
			}
			string += "]";
			ConsoleHandler::consolePrintColor(string, 6);
		}
	}
}
void ComponentObjectMemory::system(Entity& entity) {
	if (entity.entityEventHas<EventObjectSeen>()) {
		auto* eventObjectSeen = entity.entityEventGet<EventObjectSeen>();

		objectMemoryHolder.memoryUpdate(*eventObjectSeen->objectsSeen);
	}
}
void ComponentActorBlackboard::system(Entity& entity) {
	// record seen objects to blackboard
	if (entity.entityEventHas<EventObjectSeen>()) {

		auto* eventObjectSeen = entity.entityEventGet<EventObjectSeen>();

		actorBlackboard.dataSet("ObjectsSeen", *eventObjectSeen->objectsSeen);
	}
	// record memory to blackboard
	if (entity.entityComponentHas<ComponentObjectMemory>()) {

		auto* componentObjectMemory = entity.entityComponentGet<ComponentObjectMemory>();
		
		actorBlackboard.dataSet("Memory", componentObjectMemory->objectMemoryHolder);
	}
	// invoke updateFunc to do specialized data processing
	std::invoke(updateFunc, entity, actorBlackboard);
}
void ComponentActorData::system(Entity& entity) {
	if (entity.entityComponentHas<ComponentActorBlackboard>()) {

		actorDataHolder.emotionsUpdate(entity.entityComponentGet<ComponentActorBlackboard>()->actorBlackboard);
	}
}
void ComponentActorStateManager::system(Entity& entity) {

	try {
		if (!entity.entityComponentHas<ComponentActorData>()) {
			throw "Does not have ComponentActorData";
		}
		if (!entity.entityComponentHas<ComponentActorBlackboard>()) {
			throw "Does not have ComponentActorBlackboard";
		}
		if (stateManager.statesCountGet() <= 0) {
			throw "Does not have any states";
		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentActorStateManager system failed: Exception: " + std::string(e));
		return;
	}

	auto* componentActorData = entity.entityComponentGet<ComponentActorData>();
	auto* componentActorBlackboard = entity.entityComponentGet<ComponentActorBlackboard>();

	stateManager.statesUpdate(componentActorData->actorDataHolder, componentActorBlackboard->actorBlackboard);
	stateManager.stateActiveSet(0);
}
void ComponentActorStateTicker::system(Entity& entity) {

	try {
		if (!entity.entityComponentHas<ComponentActorStateManager>()) {
			throw "Does not have ComponentActorStateManager";
		}
		if (!entity.entityComponentHas<ComponentActorBlackboard>()) {
			throw "Does not have ComponentActorBlackboard";
		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentActorStateTicker system failed: Exception: " + std::string(e));
		return;
	}

	auto* componentActorStateManager = entity.entityComponentGet<ComponentActorStateManager>();
	auto* componentActorBlackboard = entity.entityComponentGet<ComponentActorBlackboard>();

	componentActorStateManager->stateManager.stateActiveUpdate(entity, componentActorBlackboard->actorBlackboard);
}
void ComponentActorMovementHandler::system(Entity& entity) {
	// handle EventActorGoTo
	if (entity.entityEventHas<EventActorGoTo>()) {
		auto events = entity.entityEventGetAllOfType<EventActorGoTo>();

		for (uint16_t i = 0; i < events.size(); i++) {
			std::invoke(goToFunction, entity, events[i]->positionTo);
		}
	}
	// handle EventActorTurnTo
	if (entity.entityEventHas<EventActorTurnTo>()) {
		auto events = entity.entityEventGetAllOfType<EventActorTurnTo>();

		for (uint16_t i = 0; i < events.size(); i++) {
			std::invoke(turnToFunction, entity, events[i]->positionTo);
		}
	}
}

#pragma endregion Systems

