#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "../ACECS/Panels.hpp"
#include "../Include/Common/DataCache.hpp"
#include <Auxiliary/Math.hpp>
#include "../Include/Game/AI/Actors/Blackboard/ActorBlackboard.hpp"
#include "../Include/Game/AI/Actors/Data/ActorDataHolder.hpp"
#include "../Include/Game/AI/Actors/Movement/ActorMovementTypes.hpp"
#include "../Include/Game/AI/Actors/Orders/OrderHandler.hpp"
#include "../Include/Game/AI/Actors/Orders/OrderTypes.hpp"
#include "../Include/Game/AI/Memory/ObjectMemoryHolder.hpp"
#include "../Include/Game/AI/Utility AI/UtilityStateManager.hpp"
#include "../Include/Game/Pathfinding/AStar/Path Creation/AStarPathfinder.hpp"
#include "../Include/Game/RayCasting/Object Vision/ObjectVision.hpp"
#include "../Include/Game/RayCasting/Vision Rendering/MemoryHolderVision.hpp"
#include "../Include/Game/RayCasting/Vision Rendering/VisionCaster.hpp"
#include "../Include/Game/World/Objects/ObjectTypes.hpp"
#include "ECS.hpp"
#include "SFML/Graphics.hpp"
#include <Audio/AudioStore.hpp>
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

		EventObjectSeen() {
			clear();
		};

		ObjectIdVector* objectsSeen;

		void clear() final {
			objectsSeen = nullptr;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventObjectSeen());
		};
	};
	// event for when a world vision component (I.E. a component that reads world textures) is updated.
	struct EventVisionUpdated final : public Event {

		EventVisionUpdated() {};

		sf::Texture textureToMemorize;

		void clear() final {
			textureToMemorize.create(1, 1);
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventVisionUpdated());
		};
	};
	struct EventActorGoTo final : public Event {

		EventActorGoTo() {
			clear();
		};

		sf::Vector2f positionTo;
		// the how close to the target to move, defaults to 0
		float desiredDist;

		void clear() final {
			positionTo = sf::Vector2f(0.f, 0.f);
			desiredDist = 0.f;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventActorGoTo());
		};
	};
	struct EventActorTurnTo final : public Event {

		EventActorTurnTo() {};

		sf::Vector2f positionTo;

		void clear() final {
			positionTo = sf::Vector2f(0.f, 0.f);
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventActorTurnTo());
		};
	};
	// event for orders given to actors by other entities
	struct EventActorOrder final : public Event {

		EventActorOrder() {};

		// the data for the order, including type of order, id of entity that gave the order, and other custom data.
		OrderData orderData;

		void clear() final {
			orderData.dataClear();
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventActorOrder());
		};
	};
	// event containing data about an order this entity is transmitting
	struct EventOrderTransmit final : public Event {

		EventOrderTransmit() {};

		OrderData orderData;

		void clear() final {
			orderData.dataClear();
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventOrderTransmit());
		};
	};
	// event for telling an entity they are being followed by another entity
	struct EventFollowed final : public Event {

		EventFollowed() { clear(); };

		// id of who is following
		EntityId followerId;
		// squared distance from the follower
		float distSqrd;

		void clear() final {
			followerId = 0;
			distSqrd = 0;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventFollowed());
		};
	};
	// event containing the objects at the mouse
	struct EventObjectsAtMouse final : public Event {

		EventObjectsAtMouse() { clear(); };

		// ids of objects at mouse
		std::set<EntityId> atMouseObjects;

		void clear() final {
			atMouseObjects.clear();
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventObjectsAtMouse());
		};
	};
	struct EventPath final : public Event {

		EventPath() { clear(); };

		AStarPath path;
		bool success;

		void clear() final {
			path.clear();
			success = false;
		}

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventPath());
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
	// registers this entity as having a dynamic sprite in the GameLevel then terminates itself
	struct ComponentSpriteDynamicRegister final : public Component {

		void system(Entity& entity) final;

		ComponentSpriteDynamicRegister() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSpriteDynamicRegister());
		};
	};
	// registers this entity as having a static sprite in the GameLevel then terminates itself
	struct ComponentSpriteStaticRegister final : public Component {

		void system(Entity& entity) final;

		ComponentSpriteStaticRegister() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSpriteStaticRegister());
		};
	};
	struct ComponentSprite final : public Component {

		void system(Entity& entity) final;

		ComponentSprite() {
			hasSystem = true;
		};
		// constructor that takes file name and extension, then loads/gets an image from the imageStore, and loads the texture with that image
		ComponentSprite(std::string _fileName, std::string _fileExtension) :
			ComponentSprite()
		{
			fileName = _fileName;
			fileExtension = _fileExtension;
			
			// if texture does exist, get/load image from the file name and extension, then create texture with that image
			if (!GraphicsStore::textureStore.objectExists(fileName)) {

				sf::Image& image = GraphicsStore::imageStore.fileGetOrLoadFromName(fileName, fileExtension);

				sf::Texture texture;
				texture.loadFromImage(image);

				GraphicsStore::textureStore.objectAddFromInstance(fileName, texture);
			}
		};
		ComponentSprite(std::string _fileName) :
			ComponentSprite(_fileName, GraphicsStore::imageStore.extensionDefaultGet())
		{};

		// the name of the file for the texture
		std::string fileName;
		// the name of the extension for the texture
		std::string fileExtension;

		sf::Sprite sprite;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentSprite(fileName, fileExtension));
		};
	};
	// creates an EventRotate every frame that linearly interpolates towards the mouse
	struct ComponentRotateToMouse final : public Component {

		void system(Entity& entity) final;

		ComponentRotateToMouse() {
			hasSystem = true;
		};
		ComponentRotateToMouse(float _turnSpeed) :
			ComponentRotateToMouse()
		{
			turnSpeed = _turnSpeed;
		};

		// turn speed in radians, note that this is speed per-second
		float turnSpeed = Mathf::PI;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentRotateToMouse(turnSpeed));
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
		float& x = position.x;
		float& y = position.y;

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
	// holds an instance of the VisionCaster class for use by ComponentVisionCasterStatic and ComponentVisionCasterDynamic
	struct ComponentVisionCasterHolder final : public Component {

		void system(Entity& entity) final;

		ComponentVisionCasterHolder() {
			hasSystem = true;
			updateCooldown = Cooldown(1.f / 60);
			doUpdate = true;
		};
		ComponentVisionCasterHolder(VisionCaster _visionCaster) :
			ComponentVisionCasterHolder()
		{
			visionCaster = _visionCaster;
		};

		VisionCaster visionCaster;

		Cooldown updateCooldown;
		bool doUpdate;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentVisionCasterHolder(visionCaster));
		};
	};
	struct ComponentVisionCasterStatic final : public Component {

		void system(Entity& entity) final;

		ComponentVisionCasterStatic() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentVisionCasterStatic());
		};
	};
	struct ComponentVisionCasterDynamic final : public Component {

		void system(Entity& entity) final;

		ComponentVisionCasterDynamic() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentVisionCasterDynamic());
		};
	};
	struct ComponentMemoryVision final : public Component {

		void system(Entity& entity) final;

		ComponentMemoryVision() {
			hasSystem = true;
		};
		ComponentMemoryVision(MemoryHolderVision _memoryHolder) :
			ComponentMemoryVision()
		{
			memoryHolder = _memoryHolder;
		};

		MemoryHolderVision memoryHolder;

		sf::Vector2f cameraMovedAmountTotal;


		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentMemoryVision(memoryHolder));
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
			objectType = ObjectType::Null;
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
	// populates the ObjectGrid with the specified type every update,
	// NOTE: this component should be ordered AFTER any movement components.
	struct ComponentObjectGridInhabiterRadius final : public Component {

		void system(Entity& entity) final;

		ComponentObjectGridInhabiterRadius() {
			hasSystem = true;
			radius = 0;
			WorldPosition{};
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
		ComponentObjectVision(Cooldown _updateCooldown) :
			ComponentObjectVision()
		{
			cooldownVisionUpdate = _updateCooldown;
		};
		ComponentObjectVision(ObjectVision _objectVision, Cooldown _updateCooldown) :
			ComponentObjectVision(_updateCooldown)
		{
			objectVision = _objectVision;
		};

		ObjectVision objectVision;

		// cooldown for when the finish updates
		Cooldown cooldownVisionUpdate;


		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectVision(objectVision, cooldownVisionUpdate));
		};
	};
	struct ComponentObjectMemory final : public Component {

		void system(Entity& entity) final;

		ComponentObjectMemory() {
			hasSystem = true;
		};

		ComponentObjectMemory(ObjectMemoryHolder _objectMemoryHolder) :
			ComponentObjectMemory()
		{
			objectMemoryHolder = _objectMemoryHolder;
		};


		ObjectMemoryHolder objectMemoryHolder;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectMemory(objectMemoryHolder));
		};
	};
	struct ComponentObjectMemoryDebug final : public Component {

		void system(Entity& entity) final;

		ComponentObjectMemoryDebug() {
			hasSystem = true;
		};

		Cooldown cooldownPrint = Cooldown(0.25f);

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectMemoryDebug());
		};
	};
	struct ComponentObjectVisionDebug final : public Component {

		void system(Entity& entity) final;

		ComponentObjectVisionDebug() {
			hasSystem = true;
		};

		Cooldown cooldownPrint = Cooldown(0.25f);

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectVisionDebug());
		};
	};
	struct ComponentActorData final : public Component {

		void system(Entity& entity) final;

		ComponentActorData() {
			hasSystem = true;
		};
		ComponentActorData(ActorDataHolder _actorDataHolder) :
			ComponentActorData()
		{
			actorDataHolder = _actorDataHolder;
		}

		ActorDataHolder actorDataHolder;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorData(actorDataHolder));
		};
	};
	struct ComponentActorBlackboard final : public Component {

		using BlackboardUpdateFunction = std::function<void(Entity& actor, ActorBlackboard& actorBlackboard)>;

		void system(Entity& entity) final;

		ComponentActorBlackboard() {
			hasSystem = true;
			updateFunc = [](Entity&, ActorBlackboard&) {};

		};
		ComponentActorBlackboard(ActorBlackboard _actorBlackboard) :
			ComponentActorBlackboard()
		{
			actorBlackboard = _actorBlackboard;
		}
		ComponentActorBlackboard(BlackboardUpdateFunction _updateFunc) :
			ComponentActorBlackboard()
		{
			updateFunc = _updateFunc;
		}
		ComponentActorBlackboard(ActorBlackboard _actorBlackboard, BlackboardUpdateFunction _updateFunc) :
			ComponentActorBlackboard()
		{
			actorBlackboard = _actorBlackboard;
			updateFunc = _updateFunc;
		}

		ActorBlackboard actorBlackboard;

		// special update function unique to each entity that handles data in the blackboard,
		// for example, the player's updateFunc will check if any seen object is a squad member, and will assign them to "SeenSquadMembers" data.
		BlackboardUpdateFunction updateFunc;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorBlackboard(actorBlackboard, updateFunc));
		};
	};
	struct ComponentActorStateManager final : public Component {

		void system(Entity& entity) final;

		ComponentActorStateManager() {
			hasSystem = true;
		};
		ComponentActorStateManager(UtilityStateManager _stateManager) :
			ComponentActorStateManager()
		{
			stateManager = _stateManager;
		}

		UtilityStateManager stateManager;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorStateManager(stateManager));
		};
	};
	struct ComponentActorWanderSelector final : public Component {

		void system(Entity& entity) final;

		ComponentActorWanderSelector() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorWanderSelector());
		};
	};
	// calls the behavior function on the ComponentActorStateManager's active state
	struct ComponentActorStateTicker final : public Component {

		void system(Entity& entity) final;

		ComponentActorStateTicker() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorStateTicker());
		};
	};
	struct ComponentActorMovementHandler final : public Component {

		void system(Entity& entity) final;

		ComponentActorMovementHandler() {
			hasSystem = true;
			movementType = ActorMovement::MovementType::Humanoid;
		};
		ComponentActorMovementHandler(ActorMovement::MovementType _movementType) :
			ComponentActorMovementHandler()
		{
			movementType = _movementType;
		}

		ActorMovement::MovementType movementType;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorMovementHandler(movementType));
		};
	};
	struct ComponentOrderTransmitByInput final : public Component {

		void system(Entity& entity) final;

		ComponentOrderTransmitByInput() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentOrderTransmitByInput());
		};
	};
	// targets orders to certain entities
	struct ComponentOrderTargeter final : public Component {

		void system(Entity& entity) final;

		ComponentOrderTargeter() {
			hasSystem = true;
		};

		std::set<EntityId> actorsTargeted;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentOrderTargeter());
		};
	};
	// targets the follow instruction of follow orders to certain entities
	struct ComponentOrderFollowTargeter final : public Component{

		void system(Entity& entity) final;

		ComponentOrderFollowTargeter() {
			hasSystem = true;
		};

		std::set<EntityId> actorsTargeted;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentOrderFollowTargeter());
		};
	};
	struct ComponentOrderTransmitter final : public Component {

		void system(Entity& entity) final;

		ComponentOrderTransmitter() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentOrderTransmitter());
		};
	};
	// draws highlights around targeted actors.
	struct ComponentOrderTargetingDrawer final : public Component {

		void system(Entity& entity) final;

		ComponentOrderTargetingDrawer() {
			hasSystem = true;
		};

		// the highlights to be drawn
		std::vector<sf::CircleShape> highlights;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentOrderTargetingDrawer());
		};
	};
	// listens for orders and adds them to the actor's blackboard if it hears them
	struct ComponentActorOrderReceiver final : public Component {

		void system(Entity& entity) final;

		ComponentActorOrderReceiver() {
			hasSystem = true;
		};

		OrderVector orderVector;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentActorOrderReceiver());
		};
	};
	// keeps track of who is following the entity
	struct ComponentFollowerTracker final : public Component {

		void system(Entity& entity) final;

		ComponentFollowerTracker() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentFollowerTracker());
		};
	};
	// get's the objects at the mouse and sends an EventObjectsAtMouse for them
	struct ComponentObjectsGetAtMouse final : public Component {

		void system(Entity& entity) final;

		ComponentObjectsGetAtMouse() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentObjectsGetAtMouse());
		};
	};
	struct ComponentAStarPathHolder final : public Component {

		void system(Entity& entity) final;

		ComponentAStarPathHolder() {
			hasSystem = true;
			updateCooldown = Cooldown(0.1f);
		};

		AStarPath path;
		// current position in the path we are moving to,
		// this is used to make sure the entity moves to the center of the cell before re-pathing
		sf::Vector2f pathTarget;
		Cooldown updateCooldown;
		// target we hope to path find to
		sf::Vector2f endTarget;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentAStarPathHolder());
		};
	};
	struct ComponentAudioPlayer final : public Component {

		void system(Entity& entity) final;

		ComponentAudioPlayer() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentAudioPlayer());
		};
	};
	struct ComponentAudioListener final : public Component {

		void system(Entity& entity) final;

		ComponentAudioListener() {
			hasSystem = true;
		};

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new ComponentAudioListener());
		};
	};
}

#endif
