#include "../Include/Game/AI/Utility AI/States/AIStates.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include <Graphics.hpp>

uint32_t MAX_ENTITIES = 100;
uint16_t MAX_COMPONENT_TYPES = 29;
uint16_t MAX_EVENT_TYPES = 10;

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
	EventRegistry::typeRegister<EventIDs<EventActorOrder>>();
	EventRegistry::typeRegister<EventIDs<EventOrderTransmit>>();

	//EventRegistry::typeRegister<EventIDs<EVENT_GOES_HERE>>();
	//EventRegistry::typeRegister<EventIDs<EVENT_GOES_HERE>>();
	//EventRegistry::typeRegister<EventIDs<EVENT_GOES_HERE>>();
}

#pragma endregion Events
#pragma region Components

// registers all components,
// registering a component gives it an ID which dictates it's update order, lower ID, sooner update.
void EntityComponents::componentIDsInitialize() {
	using ComponentRegistry = TypeIDAllocator<Component>;

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectTypeAssigner>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteDynamicRegister>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSpriteStaticRegister>>();
	
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorStateTicker>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorMovementHandler>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentMoveByInput>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentOrderTransmitByInput>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentOrderTargeter>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentOrderTransmitter>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotateToMouse>>();
	
	ComponentRegistry::typeRegister<ComponentIDs<ComponentPosition>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectGridInhabiterRadius>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentDistortionRadius>>();

	ComponentRegistry::typeRegister<ComponentIDs<ComponentSprite>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentOrderTargetingDrawer>>();
	
	ComponentRegistry::typeRegister<ComponentIDs<ComponentViewFollow>>();
	
	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();
	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();
	//ComponentRegistry::typeRegister<ComponentIDs<COMPONENT_GOES_HERE>>();
	
	// senses/memory
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectVision>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentObjectMemory>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorOrderReceiver>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentActorWanderSelector>>();

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
		"Sprite Dynamic",
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
		"Sprite Static",
		{
			"Transform",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentSpriteStaticRegister>(),
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
			createComponentPairFromType<ComponentActorData>(ActorDataHolder({50.f, 50, 25, 75}, {0}, [](const ActorBlackboard& actorBlackboard, ActorDataHolder& actorData) {
			})
			),
			createComponentPairFromType<ComponentActorStateManager>(std::vector<UtilityStates::StateBase*>{
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
			"Sprite Dynamic",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::Player),
			createComponentPairFromType<ComponentMoveByInput>(120.f),
			createComponentPairFromType<ComponentPosition>(sf::Vector2f(256.f, 256.f)),
			createComponentPairFromType<ComponentRotateToMouse>(Mathf::TAU*1.25f),
			createComponentPairFromType<ComponentVisionCasterStatic>(VisionCaster(sf::Vector2f(256.f, 256.f))),
			createComponentPairFromType<ComponentVisionCasterDynamic>(VisionCaster(sf::Vector2f(256.f, 256.f))),
			createComponentPairFromType<ComponentMemoryVision>(MemoryHolderVision(sf::Vector2f(640 * 4, 360 * 4))),
			createComponentPairFromType<ComponentViewFollow>(PanelName::GameView),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(32.f),
			createComponentPairFromType<ComponentSprite>("Art/Squad Member.png"),
			createComponentPairFromType<ComponentOrderTransmitByInput>(),
			createComponentPairFromType<ComponentOrderTargeter>(),
			createComponentPairFromType<ComponentOrderTransmitter>(),
			createComponentPairFromType<ComponentOrderTargetingDrawer>(),
		}
		);
	ComponentTemplateManager::componentTemplateAdd(

		/// template name
		"Squad Member",
		{
			"Actor",
			"Sprite Dynamic",
		},
		/// list of components in template
		{
			createComponentPairFromType<ComponentActorMovementHandler>(ActorMovement::MovementType::Humanoid),
			createComponentPairFromType<ComponentActorOrderReceiver>(),
			createComponentPairFromType<ComponentObjectTypeAssigner>(ObjectType::SquadMember),
			createComponentPairFromType<ComponentPosition>(sf::Vector2f(256.f + 64.f, 256.f)),
			createComponentPairFromType<ComponentObjectGridInhabiterRadius>(32.f),
			createComponentPairFromType<ComponentSprite>("Art/Squad Member.png"),
			createComponentPairFromType<ComponentActorBlackboard>([](Entity& actor, ActorBlackboard& actorBlackboard) {

			const float delta = TimeHandler::deltaSimulatedGet();


			auto objectsSeen = actorBlackboard.dataGet<ObjectIdVector>("ObjectsSeen");

			if (objectsSeen[uint16_t(ObjectType::Player)].size() > 0) {

				EntityId playerId = objectsSeen[uint16_t(ObjectType::Player)][0];

				// set this actor's LeaderId to the playerId.
				actorBlackboard.dataSet("LeaderId", playerId);
			}
			//if (objectsSeen[uint16_t(ObjectType::SquadMember)].size() > 0) {



			//	EntityId playerId = objectsSeen[uint16_t(ObjectType::SquadMember)][0];

			//	// set this actor's LeaderId to the playerId.
			//	actorBlackboard.dataSet("LeaderId", playerId);
			//}

			if (actorBlackboard.dataHas("Orders")) {

				OrderVector orders = actorBlackboard.dataGet<OrderVector>("Orders");

				OrderSubVector& ordersDismiss = orders[uint16_t(OrderType::OrdersDismiss)];

				if (ordersDismiss.size() > 0) {

					// iterate over OrdersDismiss orders
					for (uint16_t i = 0; i < ordersDismiss.size(); i++) {

						// get current OrdersDismiss order
						EntityId ordersToDismissId = ordersDismiss[i].dataGet<EntityId>("OrderFromId");

						// iterate over all other orders
						for (uint16_t j = 0; j < uint16_t(OrderType::SIZE); j++) {
							// skip order if it is an order dismiss order
							if (j == uint16_t(OrderType::OrdersDismiss)) continue;

							OrderSubVector& ordersSubVector = orders[j];

							// iterate over all orders in current sub vector
							for (int16_t k = ordersSubVector.size() - 1; k >= 0; k--) {

								OrderToSet orderToIds = ordersSubVector[k].dataGet<OrderToSet>("OrderToIds");

								// if an order is from the entity that told us to dismiss our orders from them, dismiss the order
								if (ordersSubVector[k].dataGet<EntityId>("OrderFromId") == ordersToDismissId && (orderToIds.contains(actor.Id) || orderToIds.size() <= 0)) {
									ordersSubVector.erase(ordersSubVector.begin() + k);
								}
							}
						}

						// erase the current OrdersDismiss order
						ordersDismiss.erase(ordersDismiss.begin() + i);
					}
				}

				// list of orders told to us
				OrderVector ordersToSelfVector;

				// populate ordersToUsVector
				for (uint16_t i = 0; i < uint16_t(OrderType::SIZE); i++) {
					ordersToSelfVector.push_back(OrderSubVector());
				}

				for (uint16_t i = 0; i < uint16_t(OrderType::SIZE); i++) {

					OrderSubVector& ordersSubVector = orders[i];

					// iterate over all orders in current sub vector
					for (uint16_t j = 0; j < ordersSubVector.size(); j++) {

						OrderToSet orderToIds = ordersSubVector[j].dataGet<OrderToSet>("OrderToIds");

						if (orderToIds.contains(actor.Id) || orderToIds.size() <= 0) {
							ordersToSelfVector[i].push_back(ordersSubVector[j]);
						}
					}
				}

				actorBlackboard.dataSet("OrdersToSelf", ordersToSelfVector);
				actorBlackboard.dataSet("Orders", orders);
			}
		}),
		createComponentPairFromType<ComponentActorData>(ActorDataHolder({50.f, 50, 25, 75}, {0}, [](const ActorBlackboard& actorBlackboard, ActorDataHolder& actorData) {

		})),
		createComponentPairFromType<ComponentActorStateManager>(UtilityStateManager(std::vector<UtilityStates::StateBase*>{
		new UtilityStates::StateIdle(),
		new UtilityStates::StateActorFollow(),
		new UtilityStates::StatePointGoTo(),
		})),
		createComponentPairFromType<ComponentActorWanderSelector>(),
		});
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
#include "../Include/Game/AI/Actors/Movement/ActorMovementFunctions.hpp"
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
		const float delta = float(TimeHandler::deltaSimulatedGet());

		const float turnSpeedDelta = turnSpeed * delta;

		auto& gameViewPanel = PanelManager::panelGet(PanelName::GameView);

		float angle = Vector2fMath::angle(entity.entityComponentGet<ComponentPosition>()->position, gameViewPanel.viewMousePositionGet());

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

		// wrap rotation between -PI and +PI
		if (rotationComponent->rotation >= +Mathf::PI) rotationComponent->rotation -= Mathf::TAU;
		if (rotationComponent->rotation <= -Mathf::PI) rotationComponent->rotation += Mathf::TAU;

		float angleDiff = angle - rotationComponent->rotation;

		// wrap angleDiff between -PI and +PI
		if (angleDiff >= +Mathf::PI) angleDiff -= Mathf::TAU;
		if (angleDiff <= -Mathf::PI) angleDiff += Mathf::TAU;

		auto* rotateEvent = entity.entityEventAddAndGet<EntityEvents::EventRotate>();

		if (angleDiff > turnSpeedDelta) {
			rotateEvent->rotateAmount = turnSpeedDelta;
		}
		else if (angleDiff < -turnSpeedDelta) {
			rotateEvent->rotateAmount = -turnSpeedDelta;
		}
		else {
			rotateEvent->rotateAmount = angleDiff;
		}
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

	if (entity.entityComponentHas<ComponentRotation>()) {
		sprite.setRotation(entity.entityComponentGet<ComponentRotation>()->rotation * 180.f / Mathf::PI);
	}
}
void ComponentOrderTargetingDrawer::system(Entity& entity) {

	highlights.clear();

	if (!entity.entityComponentHas<ComponentOrderTargeter>()) return;

	OrderToSet actorsTargeted = entity.entityComponentGet<ComponentOrderTargeter>()->actorsTargeted;

	GameLevel* gameLevel = GameLevelGrid::levelGet(entity.entityComponentGet<ComponentPosition>()->worldPosition.level);

	for (OrderToSet::iterator itr = actorsTargeted.begin(); itr != actorsTargeted.end(); itr++) {

		Entity& actor = EntityManager::entityGet(*itr);

		auto* actorComponentPosition = actor.entityComponentGet<ComponentPosition>();
		auto* actorComponentObjectGridInhabiterRadius = actor.entityComponentGet<ComponentObjectGridInhabiterRadius>();

		float radius = actorComponentObjectGridInhabiterRadius->radius;

		sf::CircleShape circleShape(radius);

		circleShape.setOrigin(sf::Vector2f(radius, radius));
		circleShape.setPosition(actorComponentPosition->position);
		circleShape.setFillColor(sf::Color::Transparent);
		circleShape.setOutlineColor(sf::Color(0, 200, 0, 127));
		circleShape.setOutlineThickness(1.f);

		highlights.push_back(circleShape);
	}
}
void ComponentVisionCasterStatic::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentRotation>() && entity.entityComponentHas<ComponentPosition>()) {

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		float coneSize = 120.f * Mathf::PI / 180.f;

		visionCaster.visionClear();
		visionCaster.textureToSeeSet(GameLevelGrid::levelGet(positionComponent->worldPosition.level)->worldTextureStatic);
		visionCaster.update(positionComponent->position.x, positionComponent->position.y, rotationComponent->rotation - (coneSize / 2.f), coneSize, 1000, 350);

		// see all around
		visionCaster.update(positionComponent->position.x, positionComponent->position.y, 0, Mathf::TAU, 64, 256);

		auto* eventVisionUpdated = entity.entityEventAddAndGet<EventVisionUpdated>();
		eventVisionUpdated->textureToMemorize = visionCaster.visionTextureGet().getTexture();
	}
}
void ComponentVisionCasterDynamic::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentRotation>() && entity.entityComponentHas<ComponentPosition>()) {

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
		auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		float coneSize = 120.f * Mathf::PI / 180.f;

		visionCaster.visionClear();
		visionCaster.textureToSeeSet(GameLevelGrid::levelGet(positionComponent->worldPosition.level)->worldTextureDynamic);
		visionCaster.update(positionComponent->position.x, positionComponent->position.y, rotationComponent->rotation - (coneSize / 2.f), coneSize, 1000, 350);

		// see all around
		visionCaster.update(positionComponent->position.x, positionComponent->position.y, 0, Mathf::TAU, 64, 256);
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

		lerp = 0.025f;

		mouseDiff.x *= lerp / (16.f / 9.f);
		mouseDiff.y *= lerp * (16.f/9.f);

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
void ComponentSpriteStaticRegister::system(Entity& entity) {
	try {
		if (!entity.entityComponentHas<ComponentSprite>()) {
			throw "Does not have ComponentSprite";
		}
		if (!entity.entityComponentHas<ComponentPosition>()) {
			throw "Does not have ComponentPosition";
		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentSpriteStaticRegister system failed: Exception:");
		return;
	}

	auto* componentPosition = entity.entityComponentGet<ComponentPosition>();

	GameLevelGrid::levelGet(componentPosition->worldPosition.level)->staticSpriteEntityIds.push_back(entity.Id);

	entity.entityComponentTerminate<ComponentSpriteStaticRegister>();
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
			ActorMovement::goTo(movementType, entity, events[i]->positionTo, events[i]->desiredDist);
		}
	}
	// handle EventActorTurnTo
	if (entity.entityEventHas<EventActorTurnTo>()) {
		auto events = entity.entityEventGetAllOfType<EventActorTurnTo>();

		for (uint16_t i = 0; i < events.size(); i++) {
			ActorMovement::turnTo(movementType, entity, events[i]->positionTo);
		}
	}
}
void ComponentOrderTransmitByInput::system(Entity& entity) {

	if (!(
		InputInterface::inputGetActive("Order 1") ||
		InputInterface::inputGetActive("Order 2") ||
		InputInterface::inputGetActive("Order 3") ||
		InputInterface::inputGetActive("Order 4") ||
		InputInterface::inputGetActive("Order 5")
		)) return;
	
	auto eventActorsTargetedVector = entity.entityEventGetAllOfType<EventOrderTransmit>();

	OrderData orderData;
	orderData.dataSet("OrderFromId", entity.Id);
	orderData.dataSet("OrderToIds", OrderToSet());

	if (InputInterface::inputGetActive("Order 1")) {
		Panel& panelGameView = PanelManager::panelGet(PanelName::GameView);

		orderData.dataSet("OrderType", OrderType::PointGoTo);
		orderData.dataSet("PointToGoTo", panelGameView.viewMousePositionGet());
	}
	else if (InputInterface::inputGetActive("Order 2")) {
		orderData.dataSet("OrderType", OrderType::ActorFollow);
		orderData.dataSet("ActorToFollowId", entity.Id);
	}
	else if (InputInterface::inputGetActive("Order 3")) {
		orderData.dataSet("OrderType", OrderType::ActorFollowTactical);
		orderData.dataSet("ActorToFollowId", entity.Id);
	}
	else if (InputInterface::inputGetActive("Order 4")) {
		orderData.dataSet("OrderType", OrderType::OrdersEnforce);
	}
	else if (InputInterface::inputGetActive("Order 5")) {
		orderData.dataSet("OrderType", OrderType::OrdersDismiss);
	}

	auto* eventOrderTransmit = entity.entityEventAddAndGet<EventOrderTransmit>();
	eventOrderTransmit->orderData = orderData;
}
void ComponentOrderTargeter::system(Entity& entity) {
	try {
		if (!entity.entityComponentHas<ComponentPosition>()) {
			throw "Does not have ComponentPosition";
		}
		else if (!entity.entityComponentHas<ComponentRotation>()) {
			throw "Does not have ComponentRotation";
		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentOrderTargeter system failed: Exception: " + std::string(e));
	}

	if (!InputInterface::inputGetActive("Order Target Select Active")) {
		actorsTargeted.clear();
		return;
	}

	if (InputInterface::inputGetActive("Order Target Select")) {

		auto* componentPosition = entity.entityComponentGet<ComponentPosition>();

		Panel& panelGameView = PanelManager::panelGet(PanelName::GameView);

		// casts a ray from the position component to the mouse
		ObjectVision toMouseVision;

		float entityToMouseDist = Vector2fMath::dist(componentPosition->position, panelGameView.viewMousePositionGet());
		float entityToMouseAngle = Vector2fMath::angle(componentPosition->position, panelGameView.viewMousePositionGet());

		toMouseVision.update(componentPosition->position.x, componentPosition->position.y, entityToMouseAngle, 0, entityToMouseDist, 1);

		// list of objects between the position component and the mouse
		ObjectIdVector& toMouseObjects = toMouseVision.objectsSeenGet();

		// iterate over every type of object
		for (uint16_t i = 0; i < uint16_t(ObjectType::SIZE); i++) {
			if (toMouseObjects[i].size() <= 0) continue;

			EntityId targetNew = *(toMouseObjects[i].end() - 1);
			// skip own Id
			if (targetNew == entity.Id) continue;

			actorsTargeted.insert(targetNew);
		}
	}

	if (actorsTargeted.size() > 0 && entity.entityEventHas<EventOrderTransmit>()) {
		auto eventOrderTransmitVector = entity.entityEventGetAllOfType<EventOrderTransmit>();

		for (uint16_t i = 0; i < eventOrderTransmitVector.size(); i++) {
			OrderToSet orderToIds = eventOrderTransmitVector[i]->orderData.dataGet<OrderToSet>("OrderToIds");

			orderToIds.insert(actorsTargeted.begin(), actorsTargeted.end());

			eventOrderTransmitVector[i]->orderData.dataSet("OrderToIds", orderToIds);

		}
	}
}
void ComponentActorWanderSelector::system(Entity& entity) {

	try {
		if (!entity.entityComponentHas<ComponentActorBlackboard>()) {
			throw "Does not have ComponentActorBlackboard";
		}
		else if (!entity.entityComponentHas<ComponentPosition>()) {
			throw "Does not have ComponentPosition";

		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentActorWanderSelector system failed: Exception: " + std::string(e));
		return;
	}

	ActorBlackboard& actorBlackboard = entity.entityComponentGet<ComponentActorBlackboard>()->actorBlackboard;
	sf::Vector2f position = entity.entityComponentGet<ComponentPosition>()->position;

	if (!actorBlackboard.dataHas("CooldownWander")) {
		actorBlackboard.dataSet("CooldownWander", Cooldown(0.f));
	}
	if (!actorBlackboard.dataHas("PointWander")) {
		actorBlackboard.dataSet("PointWander", sf::Vector2f(0.f, 0.f));
	}
	
	const float delta = TimeHandler::deltaSimulatedGet();

	Cooldown cooldownWander = actorBlackboard.dataGet<Cooldown>("CooldownWander");

	if (cooldownWander.updateAutoReset(delta)) {
		actorBlackboard.dataSet("PointWander", position + sf::Vector2f(RNGf::getRange(256.f), RNGf::getRange(256.f)));
		cooldownWander.target = RNGf::getRange(2.f, 8.f);
	}
	actorBlackboard.dataSet("CooldownWander", cooldownWander);
}
void ComponentOrderTransmitter::system(Entity& entity) {

	if (!entity.entityComponentHas<ComponentPosition>()) {
		ConsoleHandler::consolePrintErr("ComponentOrderTransmitter system failed: Exception: Does not have ComponentPosition");
		return;
	}

	if (entity.entityEventHas<EventOrderTransmit>()) {

		auto eventOrderTransmitVector = entity.entityEventGetAllOfType<EventOrderTransmit>();

		for (uint16_t eventCurInd = 0; eventCurInd < eventOrderTransmitVector.size(); eventCurInd++) {
			auto* componentPosition = entity.entityComponentGet<ComponentPosition>();

			// used for getting surrounding objects to give orders to.
			ObjectVision objectLocator;
			objectLocator.update(componentPosition->position.x, componentPosition->position.y, 0, Mathf::TAU, 520, 256);
			ObjectIdVector objects = objectLocator.objectsSeenGet();

			for (uint16_t i = 0; i < objects.size(); i++) {
				for (uint16_t j = 0; j < objects[i].size(); j++) {

					if (objects[i][j] == entity.Id) continue;

					Entity& entityCur = EntityManager::entityGet(objects[i][j]);

					auto* entityCurEventActorOrder = entityCur.entityEventAddAndGet<EntityEvents::EventActorOrder>();

					entityCurEventActorOrder->orderData = eventOrderTransmitVector[eventCurInd]->orderData;
				}
			}
		}
	}
}
void ComponentActorOrderReceiver::system(Entity& entity) {
	
	try {
		if (!entity.entityComponentHas<ComponentActorBlackboard>()) {
			throw "Does not have ComponentActorBlackboard";
		}
	}
	catch (const char* e) {
		ConsoleHandler::consolePrintErr("ComponentActorOrderListener system failed: Exception: " + std::string(e));
	}
	
	if (!entity.entityEventHas<EventActorOrder>()) return;

	ActorBlackboard& actorBlackboard = entity.entityComponentGet<ComponentActorBlackboard>()->actorBlackboard;

	auto eventActorOrderVector = entity.entityEventGetAllOfType<EventActorOrder>();

	for (uint16_t i = 0; i < eventActorOrderVector.size(); i++) {

		OrderVector orderVector;

		if (actorBlackboard.dataHas("Orders")) {
			orderVector = actorBlackboard.dataGet<OrderVector>("Orders");
		}
		else {
			// populate orderVector if it's never been initialized
			for (uint16_t i = 0; i < uint16_t(OrderType::SIZE); i++) {
				orderVector.push_back(OrderSubVector());
			}
		}

		OrderSubVector& orderSubVector = orderVector[uint16_t(eventActorOrderVector[i]->orderData.dataGet<OrderType>("OrderType"))];

		// ensure an order of the same type from the same person does not already exist,
		// if it does, override it will the new one.
		for (uint16_t i = 0; i < orderSubVector.size(); i++) {
			if (orderSubVector[i].dataGet<EntityId>("OrderFromId") == eventActorOrderVector[i]->orderData.dataGet<EntityId>("OrderFromId")) {
				orderSubVector[i] = eventActorOrderVector[i]->orderData;
				// if we replaced the existing order with our new order, we goto orderReplaced, so the new order isn't added twice.
				goto orderReplacedLabel;
			}
		}

		// added the new order to the orderSubVector
		orderSubVector.push_back(eventActorOrderVector[i]->orderData);
		// used for skipping adding of the new order to the orderSubVector
		orderReplacedLabel:

		actorBlackboard.dataSet("Orders", orderVector);
	}

}

#pragma endregion Systems

