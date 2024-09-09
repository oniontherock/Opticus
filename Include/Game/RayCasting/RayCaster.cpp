#include "RayCaster.hpp"

void RayCaster::update(float fromX, float fromY, float toAngle, float coneSize, uint32_t rayCount) {

	castPosition = sf::Vector2f(fromX, fromY);

	raysCast(toAngle, coneSize, rayCount);
}

