#include "RayCaster.hpp"

void RayCaster::update(float fromX, float fromY, float toAngle, float coneSize, float rayMaxDist, uint32_t rayCount) {

	castPosition = sf::Vector2f(fromX, fromY);
	castAngle = toAngle - (coneSize / 2.f);

	raysCast(coneSize, rayMaxDist, rayCount);
}

