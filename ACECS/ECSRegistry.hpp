#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../ACECS/Panels.hpp"
#include "../Include/Common/Math.hpp"
#include "../Include/Game/RayCasting/Vision Rendering/MemoryHolderVision.hpp"
#include "../Include/Game/RayCasting/Vision Rendering/VisionCaster.hpp"
#include "../Include/Game/World/Objects/ObjectTypes.hpp"
#include "../Include/Game/RayCasting/Object Vision/ObjectVision.hpp"
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
			naturalMovedAxis *= 0.f;
			unnaturalAxis *= 0.f;
		}
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventMoved());
		};
	};
	// event created after an update has been made to a view.
	struct EventViewMoved final : public Event {

		EventViewMoved() {};

		sf::Vector2f movedAxis{ 0,0 };
		sf::Vector2f naturalMovedAxis{ 0,0 };

		void clear() final {
			movedAxis *= 0.f;
			naturalMovedAxis *= 0.f;
		}
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventViewMoved());
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
	// contains a list of EntityIds and ObjectTypes that were seen this update.
	struct EventObjectSeen final : public Event {

		EventObjectSeen() {};

		std::set<EntityIdObjectTypePair> objectsSeen;

		void clear() final {
			objectsSeen.clear();
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventObjectSeen());
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
	// creates an EventRotate every frame that linearly interpolates towards the mouse
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

		// speed of linear interpolation towards mouse.
		// note that this is in seconds, so a speed of 0.5 means it will reach halfway in one second
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
		ComponentVisionDrawer(VisionCaster _visionCaster, MemoryHolderVision _memoryHolder) :
			ComponentVisionDrawer()
		{
			visionCaster = _visionCaster;
			memoryHolder = _memoryHolder;
		};

		VisionCaster visionCaster;
		MemoryHolderVision memoryHolder;


		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentVisionDrawer(visionCaster, memoryHolder));
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
	// assigns the ObjectType in the ObjectRegistry for the entity to be the specified objectType, then deletes itself
	struct ComponentObjectTypeAssigner final : public Component {

		void system(Entity& entity) final;

		ComponentObjectTypeAssigner() {
			hasSystem = true;
		};
		ComponentObjectTypeAssigner(ObjectType _objectType) :
			ComponentObjectTypeAssigner()
		{
			objectType = _objectType;
		};
		// ObjectType to assign to the entity
		ObjectType objectType;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectTypeAssigner(objectType));
		};
	};
	// depopulates the ObjectGrid with an entity's id in a radius.
	// entity must have ObjectType for this component to function, object type can be assigned with the ComponentObjectTypeAssigner.
	//
	// depopulation occurs in a radius around the entity, if the entity does not have a position component, this Component will not function, and will terminate itself.
	// 
	// NOTE: this component should be ordered BEFORE any movement components.
	struct ComponentObjectGridDepopulatorRadius final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridDepopulatorRadius() {
			hasSystem = true;
		};
		ComponentObjectGridDepopulatorRadius(float _radius) :
			ComponentObjectGridDepopulatorRadius()
		{
			radius = _radius;
		};

		float radius;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridDepopulatorRadius(radius));
		};
	};
	// populates the ObjectGrid with an entity's id in a radius.
	// entity must have ObjectType for this component to function, object type can be assigned with the ComponentObjectTypeAssigner.
	//
	// population occurs in a radius around the entity, if the entity does not have a position component, this Component will not function, and will terminate itself.
	// 
	// NOTE: this component should be ordered AFTER any movement components.
	struct ComponentObjectGridPopulatorRadius final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridPopulatorRadius() {
			hasSystem = true;
		};
		ComponentObjectGridPopulatorRadius(float _radius) :
			ComponentObjectGridPopulatorRadius()
		{
			radius = _radius;
		};

		float radius;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridPopulatorRadius(radius));
		};
	};
	// similar to the ComponentObjectGridPopulatorRadius and the ComponentObjectGridDepopulatorRadius,
	// but it performs the functions of both components on it's own.
	// 
	// should generally be prefered over using the manual populator/depopulator components unless they are needed for special purposes.
	// 
	// NOTE: this component should be ordered AFTER any movement components.
	struct ComponentObjectGridInhabiterRadius final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridInhabiterRadius() {
			hasSystem = true;
		};
		ComponentObjectGridInhabiterRadius(float _radius) :
			ComponentObjectGridInhabiterRadius()
		{
			radius = _radius;
		};

		float radius;
		// previous WorldPosition of population, used for depopulation.
		WorldPosition positionPrev;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectGridInhabiterRadius(radius));
		};
	};
	// gets a list of seen objects using an ObjectVision and creates an EventObjectSeen with them.
	struct ComponentObjectVision final : public Component {

		void system(Entity& entity) final;

		ComponentObjectVision() {
			hasSystem = true;
		};
		ComponentObjectVision(ObjectVision _objectVision) :
			ComponentObjectVision()
		{
			objectVision = _objectVision;
		};

		ObjectVision objectVision;


		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectVision(objectVision));
		};
	};
	struct ComponentDebug final : public Component {

		void system(Entity& entity) final;

		ComponentDebug() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentDebug());
		};
	};
}

#endif