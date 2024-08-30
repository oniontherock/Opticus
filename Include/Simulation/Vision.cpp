#include "Vision.hpp"
#include "Image Grid/WorldImageGrid.hpp"
#include "Distortions/WorldDistortionGrid.hpp"
#include "../Common/Math.hpp"
#include <iostream>


Vision::Vision() {
	screenSize = sf::Vector2u(1, 1);

	visionImage.create(screenSize.x, screenSize.y);
	memoryTexture.create(screenSize.x, screenSize.y);
}
Vision::Vision(uint16_t sizeX, uint16_t sizeY) {
	screenSize = sf::Vector2u(sizeX, sizeY);

	visionImage.create(screenSize.x, screenSize.y);
	memoryTexture.create(screenSize.x, screenSize.y);
}
Vision::Vision(const Vision& other) {
	screenSize = other.screenSize;

	visionImage.create(screenSize.x, screenSize.y);

	memoryTexture.create(screenSize.x, screenSize.y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));
}
void Vision::operator= (const Vision& other) {
	screenSize = other.screenSize;

	visionImage.create(screenSize.x, screenSize.y);

	memoryTexture.create(screenSize.x, screenSize.y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));
}

sf::Image& Vision::visionProcess(float posX, float posY, float rotation) {

	const sf::Color clearColor = sf::Color(0, 0, 0, 0);

	for (uint16_t x = 0; x < screenSize.x; x++) {
		for (uint16_t y = 0; y < screenSize.y; y++) {
			visionImage.setPixel(x, y, clearColor);
		}
	}

	memoryBlur();
	visionUpdate(posX, posY, rotation - (coneAngle / 2.f));
	visionMemorize();

	return visionImage;
}

const sf::Texture& Vision::memoryGet() {
	return memoryTexture.getTexture();
}

void Vision::visionUpdate(float posX, float posY, float rotation) {

	const sf::Vector2f rayPositionStarting(posX, posY);

	static uint32_t subConeRayAnchorCur = 0;

	auto& worldImage = WorldImageGrid::worldImageFromPixel(0, 0);

	for (uint32_t rayCur = 0; rayCur < rayCountTotal; rayCur++) {

		const float rayRotation = rotation + (rayCur * rayAngleDifference);
		const sf::Vector2f rayHeadingOrig = sf::Vector2f(cos(rayRotation), sin(rayRotation));
		
		sf::Vector2f rayHeading = rayHeadingOrig;
		sf::Vector2f rayPosition = rayPositionStarting;

		float dist = 0;
		constexpr float desiredDist = 300;

		while (dist <= desiredDist) {


			WorldDistortionGrid::headingApplyDistortionSafe(rayHeading, uint32_t(rayPosition.x), uint32_t(rayPosition.y));
			rayPosition += rayHeading;
			dist += 1.f;

			if (rayPosition.x < 0 || rayPosition.x >= screenSize.x || rayPosition.y < 0 || rayPosition.y >= screenSize.y) break;

			sf::Color worldColor = worldImage.getPixel(rayPosition.x, rayPosition.y);

			const sf::Vector2f rayVisionPos = rayPositionStarting + (rayHeadingOrig * dist);

			if (!(rayVisionPos.x < 0 || rayVisionPos.x >= screenSize.x || rayVisionPos.y < 0 || rayVisionPos.y >= screenSize.y)) {
				visionImage.setPixel(rayVisionPos.x, rayVisionPos.y, worldColor);
			}
		}
	}
}

void Vision::visionMemorize() {
	sf::Texture visionTexture;
	visionTexture.create(screenSize.x, screenSize.y);
	visionTexture.loadFromImage(visionImage);

	sf::Sprite visionSprite;
	visionSprite.setTexture(visionTexture);

	memoryTexture.draw(visionSprite);
	memoryTexture.display();
}

void Vision::memoryBlur() {
	sf::Sprite memorySprite;
	memorySprite.setTexture(memoryTexture.getTexture());

	sf::Shader blurShader;
	blurShader.loadFromFile("Include/Shaders/Blur.glsl", sf::Shader::Fragment);
	blurShader.setParameter("dir", sf::Vector2f(1, 1));
	blurShader.setParameter("blur", 2048);

	memoryTexture.draw(memorySprite, &blurShader);
	memoryTexture.display();
}

