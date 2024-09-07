#ifndef __VISION_RENDERER_H__
#define __VISION_RENDERER_H__

#include <SFML/Graphics.hpp>
#include <bitset>
#include <cstdint>
#include "../../Common/Math.hpp"

struct VisionRenderer {

	VisionRenderer();
	VisionRenderer(uint16_t screenSizeX, uint16_t screenSizeY, uint16_t worldSizeX, uint16_t worldSizeY);
	VisionRenderer(sf::Vector2u _screenSize, sf::Vector2u _worldSize);
	VisionRenderer(const VisionRenderer& other);
	void operator= (const VisionRenderer& other);

	sf::Image& visionProcess(float posX, float posY, float rotation, uint32_t rayCountTotal, float coneAngleDegrees, const sf::FloatRect viewRect);
	sf::Image& visionProcess(sf::Vector2f pos, float rotation, uint32_t rayCountTotal, float coneAngleDegrees, const sf::FloatRect viewRect);
	const sf::Texture& memoryGet();

	sf::Vector2u screenSize;
	sf::Vector2u worldSize;

private:

	sf::Image visionImage;
	sf::RenderTexture memoryTexture;

	void visionUpdate(float posX, float posY, float rotation, uint32_t rayCountTotal, float coneAngleRadians, const sf::FloatRect viewRect);
	void visionMemorize();
	void memoryBlur();
};



#endif