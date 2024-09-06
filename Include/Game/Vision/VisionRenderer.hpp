#ifndef __VISION_RENDERER_H__
#define __VISION_RENDERER_H__

#include <SFML/Graphics.hpp>
#include <bitset>
#include <cstdint>
#include "../../Common/Math.hpp"

struct VisionRenderer {

	VisionRenderer();
	VisionRenderer(uint16_t sizeX, uint16_t sizeY);
	VisionRenderer(sf::Vector2u size);
	VisionRenderer(const VisionRenderer& other);
	void operator= (const VisionRenderer& other);

	sf::Image& visionProcess(float posX, float posY, float rotation, uint32_t rayCountTotal, float coneAngleDegrees);
	sf::Image& visionProcess(sf::Vector2f pos, float rotation, uint32_t rayCountTotal, float coneAngleDegrees);
	const sf::Texture& memoryGet();
private:

	sf::Vector2u screenSize;

	sf::Image visionImage;
	sf::RenderTexture memoryTexture;

	void visionUpdate(float posX, float posY, float rotation, uint32_t rayCountTotal, float coneAngleRadians);
	void visionMemorize();
	void memoryBlur();
};



#endif