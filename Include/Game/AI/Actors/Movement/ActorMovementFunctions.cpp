#include "ActorMovementFunctions.hpp"
#include "../../../ACECS/ECSRegistry.hpp"
#include "../../../Pathfinding/AStar/Grid/AStarGrid.hpp"
#include "../../../ACECS/GameLevel.hpp"
#include "../../../../Debugging/AStarPathDrawer.hpp"

using namespace ActorMovement;
using namespace EntityEvents;
using namespace EntityComponents;

void ActorMovement::goTo(MovementType type, Entity& actor, sf::Vector2f positionTo, float desiredDist) {

	using enum MovementType;

	switch (type) {
	case Humanoid:
		goToHumanoid(actor, positionTo, desiredDist);
		break;
	}
}
void ActorMovement::turnTo(MovementType type, Entity& actor, sf::Vector2f positionTo) {
	using enum MovementType;

	switch (type) {
	case Humanoid:
		turnToHumanoid(actor, positionTo);
		break;
	}
}

void ActorMovement::goToHumanoid(Entity& actor, sf::Vector2f positionTo, float desiredDist) {
	auto* componentPosition = actor.entityComponentGet<ComponentPosition>();

	// return if we are already close enough to target
	if (Vector2fMath::distSqrd(componentPosition->position, positionTo) < desiredDist * desiredDist) return;

	float delta = float(TimeHandler::deltaSimulatedGet());

	constexpr float moveSpeed = 120.f;

	auto* eventMove = actor.entityEventAddAndGet<EntityEvents::EventMove>();

	eventMove->moveAxis = Vector2fMath::dir(componentPosition->position, positionTo) * moveSpeed * delta;
}
void ActorMovement::turnToHumanoid(Entity& actor, sf::Vector2f positionTo) {
	const float delta = float(TimeHandler::deltaSimulatedGet());

	constexpr float turnSpeed = (180.f * Mathf::PI / 180.f);

	const float turnSpeedDelta = turnSpeed * delta;

	const float angle = Vector2fMath::angle(actor.entityComponentGet<ComponentPosition>()->position, positionTo);

	auto* rotationComponent = actor.entityComponentGet<ComponentRotation>();

	// wrap rotation between -PI and +PI
	if (rotationComponent->rotation >= +Mathf::PI) rotationComponent->rotation -= Mathf::TAU;
	if (rotationComponent->rotation <= -Mathf::PI) rotationComponent->rotation += Mathf::TAU;

	float angleDiff = angle - rotationComponent->rotation;

	// wrap angleDiff between -PI and +PI
	if (angleDiff >= +Mathf::PI) angleDiff -= Mathf::TAU;
	if (angleDiff <= -Mathf::PI) angleDiff += Mathf::TAU;

	auto* rotateEvent = actor.entityEventAddAndGet<EntityEvents::EventRotate>();

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