#ifndef __ACTOR_MOVEMENT_FUNCTIONS_H__
#define __ACTOR_MOVEMENT_FUNCTIONS_H__

#include <SFML/System/Vector2.hpp>
#include <ECS/Entities/Entity.hpp>
#include "ActorMovementTypes.hpp"

namespace ActorMovement {

	// chooses the corresponding goTo function for the type
	void goTo(MovementType type, Entity& actor, sf::Vector2f positionTo, float desiredDist);
	// chooses the corresponding turnTo function for the type
	void turnTo(MovementType type, Entity& actor, sf::Vector2f positionTo);

	void goToHumanoid(Entity& actor, sf::Vector2f positionTo, float desiredDist);
	void turnToHumanoid(Entity& actor, sf::Vector2f positionTo);

}


#endif