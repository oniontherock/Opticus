#include "ECSRegistry.hpp"

uint32_t MAX_ENTITIES = 100;
uint16_t MAX_COMPONENT_TYPES = 8;
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
	ComponentRegistry::typeRegister<ComponentIDs<ComponentRotation>>();
	ComponentRegistry::typeRegister<ComponentIDs<ComponentSprite>>();
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
			createComponentPairFromType<ComponentMoveByInput>(480.f),
			createComponentPairFromType<ComponentRotateToMouse>(0.20f),
			//createComponentPairFromType<ComponentSprite>("Art/Character.png"),
			createComponentPairFromType<ComponentVisionDrawer>(Mathf::TAU / 8.f),
		}
		);
}

#pragma endregion Component Templates
#pragma region Systems

using namespace EntityComponents;
using namespace EntityEvents;

// if you need to include a certain file for a system, include it here.
#include <iostream>
#include "../Include/Common/VectorMath.hpp"
#include "../Include/Common/TimeHandler.hpp"
#include "../Include/Simulation/RayCast.hpp"
#include "../Include/Simulation/WorldImageGrid.hpp"
#include "../Include/Simulation/Distortions/WorldDistortionGrid.hpp"
#include "Input.hpp"
#include "Panels.hpp"
#include "Graphics.hpp"
#include "../Include/Shaders/Compute.hpp"
#include <numeric>
#include <glad/glad.h>


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

		auto& gameViewPanel = PanelManager::panelGet(PanelName::GameView);

		float angle = Vector2fMath::angle(entity.entityComponentGet<ComponentPosition>()->position, gameViewPanel.viewMousePositionGet());

		auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();

		float angleDiff = angle - rotationComponent->rotation;

		if (angleDiff > Mathf::PI) angleDiff -= Mathf::TAU;
		if (angleDiff < -Mathf::PI) angleDiff += Mathf::TAU;

		auto* rotateEvent = entity.entityEventAddAndGet<EventRotate>();

		rotateEvent->rotateAmount = angleDiff * lerpSpeed;
	}
}

void ComponentPosition::system(Entity& entity) {
	if (entity.entityEventHas<EventMove>()) {

		auto moveEvents = entity.entityEventGetAllOfType<EventMove>();
	
		for (uint32_t i = 0; i < moveEvents.size(); i++) {
			position += moveEvents[i]->moveAxis;
		}
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

	sf::Image testImage = spriteTexture.copyToImage();

	auto& worldImage = WorldImageGrid::worldImageFromPixel(0, 0);

	for (uint32_t x = 0; x < testImage.getSize().x; x++) {
		for (uint32_t y = 0; y < testImage.getSize().y; y++) {
			worldImage.setPixel(x + uint32_t(positionComponent->position.x), y + uint32_t(positionComponent->position.y), testImage.getPixel(x, y));
		}
	}
}
void ComponentVisionDrawer::system(Entity& entity) {

	if (entity.entityComponentHas<ComponentRotation>() && entity.entityComponentHas<ComponentPosition>()) {

		// the amount of rays
		constexpr uint32_t rayCount = 512;
		// angular difference between two ray rotations
		float rayAngleDifference = visionConeSize / rayCount;

		// the starting rotation of every ray
		std::vector<float> rayRotations = std::vector<float>(rayCount);

		for (uint32_t i = 0; i < rayCount; i++) {
			rayRotations[i] = (float(i) * rayAngleDifference) - (visionConeSize / 2.f);
		}

		constexpr uint32_t worldWidth = 1280;
		constexpr uint32_t worldHeight = 720;

		// initialise compute stuff
		Compute compute_shader("Include/Shaders/VisionRaycaster.glsl", sf::Vector2u(rayCount, 1), sf::Vector2u(worldWidth, worldHeight));
		compute_shader.use();

		auto& worldImage = WorldImageGrid::worldImageFromPixel(0, 0);
		std::vector<float> worldImageInputData = std::vector<float>(worldWidth * worldHeight * 4u);

		for (uint32_t i = 0; i < worldImageInputData.size(); i += 4) {

			uint32_t coord = i / 4u;

			uint32_t x = coord % worldWidth;
			uint32_t y = coord / worldWidth;

			sf::Color color = worldImage.getPixel(x, y);

			worldImageInputData[i + 0] = float(color.r) / 255.f;
			worldImageInputData[i + 1] = float(color.g) / 255.f;
			worldImageInputData[i + 2] = float(color.b) / 255.f;
			worldImageInputData[i + 3] = float(color.a) / 255.f;
		}

		compute_shader.set_values(worldImageInputData.data());

		std::vector<float> transformInputData = std::vector<float>(rayCount * 4u);

		for (uint32_t i = 0; i < transformInputData.size(); i += 4) {

			uint32_t coord = i / 4u;

			transformInputData[i + 0] = 320.f;
			transformInputData[i + 1] = 180.f;
			transformInputData[i + 2] = rayRotations[coord];
			transformInputData[i + 3] = 0.f;
		}

		compute_shader.set_transform(transformInputData.data());

		compute_shader.use();
		compute_shader.dispatch(1, 1);
		compute_shader.wait();   

		std::vector<float> worldImageOutputData = compute_shader.get_values();

		sf::Image visionImage;
		visionImage.create(worldWidth, worldHeight);

		for (uint32_t i = 0; i < worldImageOutputData.size(); i += 4) {

			sf::Color color = sf::Color(
				worldImageOutputData[i + 0] * 255.f,
				worldImageOutputData[i + 1] * 255.f,
				worldImageOutputData[i + 2] * 255.f,
				worldImageOutputData[i + 3] * 255.f
			);

			uint32_t coord = i / 4u;
			
			uint32_t x = coord % worldWidth;
			uint32_t y = coord / worldWidth;

			visionImage.setPixel(x, y, color);
		}
		//std::cout << "burger" << std::endl;

		////auto* rotationComponent = entity.entityComponentGet<ComponentRotation>();
		////auto* positionComponent = entity.entityComponentGet<ComponentPosition>();

		////auto& worldImage = WorldImageGrid::worldImageFromPixel(0, 0);

		////sf::Image viewImage;
		//viewImage.create(worldImage.getSize().x, worldImage.getSize().y);

		//float coneStart = -visionConeSize / 2.f;
		//float coneEnd = visionConeSize / 2.f;

		//constexpr float angleMoveAmountHalf = angleMoveAmount / 2.f;

		//while (coneStart < coneEnd) {

		//	float startDist = 0.f;
		//	float endDist = 600;
		//	float distMoveAmount = 1.f;

		//	RayCast raycast;
		//	raycast.create(positionComponent->position.x, positionComponent->position.y, cos(rotationComponent->rotation + coneStart), sin(rotationComponent->rotation + coneStart));
		//	auto colors = raycast.launch(endDist);
		//	
		//	int i = 0;

		//	while (startDist < endDist) {

		//		for (float extraAngle = -angleMoveAmountHalf; extraAngle < angleMoveAmountHalf; extraAngle += angleMoveAmountHalf / 2.f) {

		//			float axisX = cos(rotationComponent->rotation + coneStart + extraAngle) * startDist;
		//			float axisY = sin(rotationComponent->rotation + coneStart + extraAngle) * startDist;

		//			float scanX = positionComponent->position.x + axisX;
		//			float scanY = positionComponent->position.y + axisY;

		//			if (scanX >= 0 && scanX < viewImage.getSize().x && scanY >= 0 && scanY < viewImage.getSize().y) {
		//				viewImage.setPixel(uint32_t(scanX), uint32_t(scanY), colors[i]);
		//			}
		//		}

		//		startDist += distMoveAmount;
		//		i++;
		//	}

		//	coneStart += angleMoveAmount;
		//}



		sf::Texture worldTexture;
		worldTexture.loadFromImage(visionImage);

		sf::Sprite worldSprite;
		worldSprite.setTexture(worldTexture);

		PanelManager::panelGet(PanelName::GameView).objectDraw(worldSprite);
	}
}


#pragma endregion Systems

