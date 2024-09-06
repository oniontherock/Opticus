#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../Include/Common/Math.hpp"
#include "../Include/Game/Vision/VisionRenderer.hpp"
#include "../ACECS/Panels.hpp"
#include "ECS.hpp"
#include "SFML/Graphics.hpp"
#include <functional>

namespace ECSRegistry {
	void ECSInitialize();
	void ECSTerminate();
}

// avoid having undefined constructor arguments for events or components,
// as it's more convenient to not have to define every event/component all the time.
// whenever you create a new type, ensure you register it in the implementation file of this header

namespace EntityEvents {
	struct EventMove final : public Event {

		EventMove() {};

		sf::Vector2f moveAxis{ 0,0 };

		void clear() final {
			moveAxis *= 0.f;
		}
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventMove());
		};
	};
	struct EventRotate final : public Event {

		EventRotate() {};

		float rotateAmount = 0.f;

		void clear() final {
			rotateAmount = 0.f;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventRotate());
		};
	};

}
namespace EntityComponents {

	struct ComponentMoveByInput final : public Component {

		void system(Entity& entity) final;

		ComponentMoveByInput() {
			hasSystem = true;
		};
		ComponentMoveByInput(float _moveSpeed) :
			ComponentMoveByInput()
		{
			moveSpeed = _moveSpeed;
		};

		float moveSpeed = 1.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentMoveByInput(moveSpeed));
		};
	};
	struct ComponentSprite final : public Component {

		void system(Entity& entity) final;

		ComponentSprite() {
			hasSystem = true;
		};
		ComponentSprite(std::string filepath) :
			ComponentSprite()
		{
			spriteTexture.loadFromFile(filepath);
		};
		ComponentSprite(sf::Texture _spriteTexture) :
			ComponentSprite()
		{
			spriteTexture = _spriteTexture;
		};

		sf::Texture spriteTexture;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSprite(spriteTexture));
		};
	};
	struct ComponentRotateToMouse final : public Component {

		void system(Entity& entity) final;

		ComponentRotateToMouse() {
			hasSystem = true;
		};
		ComponentRotateToMouse(float _lerpSpeed) :
			ComponentRotateToMouse()
		{
			lerpSpeed = _lerpSpeed;
		};

		float lerpSpeed = 0.5f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentRotateToMouse(lerpSpeed));
		};
	};
	struct ComponentPosition final : public Component {

		void system(Entity& entity) final;

		ComponentPosition() {
			hasSystem = true;
		};
		ComponentPosition(sf::Vector2f _position) :
			ComponentPosition()
		{
			position = _position;
		};

		sf::Vector2f position;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentPosition(position));
		};
	};
	struct ComponentRotation final : public Component {

		void system(Entity& entity) final;

		ComponentRotation() {
			hasSystem = true;
		};
		ComponentRotation(float _rotation) :
			ComponentRotation()
		{
			rotation = _rotation;
		};

		float rotation = 0.f;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentRotation(rotation));
		};
	};
	struct ComponentVisionRenderer final : public Component {

		void system(Entity& entity) final;

		ComponentVisionRenderer() {
			hasSystem = true;
		};
		ComponentVisionRenderer(VisionRenderer _vision) :
			ComponentVisionRenderer()
		{
			visionRenderer = _vision;
		};

		VisionRenderer visionRenderer;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentVisionRenderer(visionRenderer));
		};
	};
	struct ComponentViewFollow final : public Component {

		void system(Entity& entity) final;

		ComponentViewFollow() {
			hasSystem = true;
			panelViewToFollow = PanelName::GameView;
		};
		ComponentViewFollow(PanelName _panelViewToFollow) :
			ComponentViewFollow()
		{
			panelViewToFollow = _panelViewToFollow;
		};

		PanelName panelViewToFollow;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentViewFollow(panelViewToFollow));
		};
	};
}

#endif