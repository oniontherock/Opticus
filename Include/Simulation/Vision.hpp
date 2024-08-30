#ifndef __VISION_H__
#define __VISION_H__

#include <SFML/Graphics.hpp>
#include <bitset>
#include <cstdint>
#include "../Common/Math.hpp"

struct Vision {

	Vision();
	Vision(uint16_t sizeX, uint16_t sizeY);
	Vision(const Vision& other);
	void operator= (const Vision& other);

	sf::Image& visionProcess(float posX, float posY, float rotation);
	const sf::Texture& memoryGet();
private:

	static constexpr uint32_t rayCountTotal = 512;
	static constexpr float coneAngle = Mathf::TAU / 8.f;
	static constexpr float rayAngleDifference = coneAngle / rayCountTotal;

	sf::Vector2u screenSize;

	sf::Image visionImage;
	sf::RenderTexture memoryTexture;

	void visionUpdate(float posX, float posY, float rotation);
	void visionMemorize();
	void memoryBlur();
};



#endif