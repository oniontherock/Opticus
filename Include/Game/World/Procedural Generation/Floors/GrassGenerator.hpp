#ifndef __GRASS_GENERATOR_H__
#define __GRASS_GENERATOR_H__

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdint>

struct GrassBlade {
	sf::Vector3f base;
	sf::Vector3f end;
	sf::Vector3f endTarget;

	float length;
	float sizeBase;
	float sizeEnd;
	
	sf::Color color;
};

struct GrassGenerator {

	GrassGenerator(sf::FloatRect area, uint32_t count);

	std::vector<sf::ConvexShape> grassSprites;

	std::vector<GrassBlade> grassBladeVector;

	void simulate(float windAngle);
	void grassSpritesUpdate();


};


#endif
