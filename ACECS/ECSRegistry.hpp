#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../Include/Common/Math.hpp"
#include "../Include/Game/RayCasting/VisionCaster.hpp"
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
	// event created after an update has been made to a ComponentPosition's position.
	// contains data for the actual movement, along with the unnatural movement.
	struct EventMoved final : public Event {

		EventMoved() {};

		sf::Vector2f movedAxis{ 0,0 };
		sf::Vector2f naturalMovedAxis{ 0,0 };
		sf::Vector2f unnaturalAxis{ 0,0 };

		void clear() final {
			movedAxis *= 0.f;
			unnaturalAxis *= 0.f;
		}
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventMoved());
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
			if (!texture.loadFromFile(filepath)) {
				ConsoleHandler::consolePrintErr("Texture loading failed! Invalid file path: \"" + filepath + "\"");
			}
		};
		ComponentSprite(sf::Texture _texture) :
			ComponentSprite()
		{
			texture = _texture;
		};

		sf::Texture texture;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSprite(texture));
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
		ComponentPosition(WorldPosition _worldPosition) :
			ComponentPosition()
		{
			worldPosition = _worldPosition;
			position = worldPosition.position;
		};

		WorldPosition worldPosition;
		sf::Vector2f& position = worldPosition.position;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentPosition(worldPosition));
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
	struct ComponentVisionDrawer final : public Component {

		void system(Entity& entity) final;

		ComponentVisionDrawer() {
			hasSystem = true;
		};
		ComponentVisionDrawer(VisionCaster _visionCaster) :
			ComponentVisionDrawer()
		{
			visionCaster = _visionCaster;
		};

		VisionCaster visionCaster;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentVisionDrawer(visionCaster));
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
	struct ComponentDistortionRadius final : public Component {

		void system(Entity& entity) final;

		ComponentDistortionRadius() {
			hasSystem = true;
		};
		ComponentDistortionRadius(float _distortionRadius, Distortion _distortion) :
			ComponentDistortionRadius()
		{
			distortionRadius = _distortionRadius;
			distortion = _distortion;
		};

		float distortionRadius = 0.f;
		Distortion distortion;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentDistortionRadius(distortionRadius, distortion));
		};
	};
}

#endif