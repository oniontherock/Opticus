#include "RayCast.hpp"

#include "../Common/VectorMath.hpp"
#include "Distortions/WorldDistortionGrid.hpp"
#include "WorldImageGrid.hpp"

RayCast::RayCast() :
	position(sf::Vector2(0, 0)),
	heading(sf::Vector2(0, 0))
{
}

#pragma region create overloads
void RayCast::create(RayVector _position) {
	position = _position;
}
void RayCast::create(RayVector _position, RayVector _heading) {
	create(_position);
	heading = _heading;
}
void RayCast::create(float _positionX, float _positionY, RayVector _heading) {
	create(RayVector(_positionX, _positionY), _heading);
}
void RayCast::create(RayVector _position, float _headingX, float _headingY) {
	create(_position, RayVector(_headingX, _headingY));
}
void RayCast::create(float _positionX, float _positionY, float _headingX, float _headingY) {
	create(RayVector(_positionX, _positionY), RayVector(_headingX, _headingY));
}
void RayCast::create(float _positionX, float _positionY) {
	create(RayVector(_positionX, _positionY));
}
#pragma endregion

std::vector<sf::Color> RayCast::launch(float dist) {

	std::vector<sf::Color> colors;

	heading = Vector2fMath::normalize(heading);

	float totalMoveAmount = 0.f;

	while (totalMoveAmount <= dist) {

		step();
		totalMoveAmount += 1.f;

		colors.push_back(WorldImageGrid::pixelGetColorSafe(uint32_t(position.x), uint32_t(position.y)));
	}

	return colors;
}

void RayCast::step() {
	WorldDistortionGrid::headingApplyDistortionSafe(heading, sf::Vector2i(position));
	position += heading;
}


