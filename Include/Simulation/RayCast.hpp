#ifndef __RAY_CAST_H__
#define __RAY_CAST_H__

#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include <vector>

using RayVector = sf::Vector2f;

struct RayCast {

	RayVector position;
	RayVector heading;

	RayCast();

	void create(RayVector _position);
	void create(RayVector _position, RayVector _heading);
	void create(float _positionX, float _positionY, RayVector _heading);
	void create(RayVector _position, float _headingX, float _headingY);
	void create(float _positionX, float _positionY);
	void create(float _positionX, float _positionY, float _headingX, float _headingY);

	std::vector<sf::Color> launch(float dist);

private:
	void step();
};



#endif