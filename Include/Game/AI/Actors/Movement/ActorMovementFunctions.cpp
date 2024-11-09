#include "ActorMovementFunctions.hpp"

using namespace ActorMovement;

void ActorMovement::goTo(MovementType type, Entity& actor, sf::Vector2f positionTo, float desiredDist) {

	//using enum class MovementType;

	switch (type) {
	case MovementType::Humanoid:

		break;
	}
}
void ActorMovement::turnTo(MovementType type, Entity& actor, sf::Vector2f positionTo) {
}

void ActorMovement::goToHumanoid(Entity& actor, sf::Vector2f positionTo, float desiredDist) {
}
void ActorMovement::turnToHumanoid(Entity& actor, sf::Vector2f positionTo) {

}