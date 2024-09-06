#include "VisionRenderer.hpp"
#include "../World/Image Grid/WorldImageGrid.hpp"
#include "../World/Distortions/WorldDistortionGrid.hpp"
#include "../../Common/Math.hpp"
#include "../../../ACECS/GameLevel.hpp"
#include <iostream>


VisionRenderer::VisionRenderer() {
	screenSize = sf::Vector2u(1, 1);

	visionImage.create(screenSize.x, screenSize.y);
	memoryTexture.create(screenSize.x, screenSize.y);
}
VisionRenderer::VisionRenderer(uint16_t sizeX, uint16_t sizeY) {
	screenSize = sf::Vector2u(sizeX, sizeY);

	visionImage.create(screenSize.x, screenSize.y);
	memoryTexture.create(screenSize.x, screenSize.y);
}
VisionRenderer::VisionRenderer(sf::Vector2u size) {
	screenSize = size;

	visionImage.create(screenSize.x, screenSize.y);
	memoryTexture.create(screenSize.x, screenSize.y);
}
VisionRenderer::VisionRenderer(const VisionRenderer& other) {
	screenSize = other.screenSize;

	visionImage.create(screenSize.x, screenSize.y);

	memoryTexture.create(screenSize.x, screenSize.y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));
}
void VisionRenderer::operator= (const VisionRenderer& other) {
	screenSize = other.screenSize;

	visionImage.create(screenSize.x, screenSize.y);

	memoryTexture.create(screenSize.x, screenSize.y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));
}
sf::Image& VisionRenderer::visionProcess(float posX, float posY, float rotation, uint32_t rayCountTotal, float coneAngleDegrees) {

	const sf::Color clearColor = sf::Color(0, 0, 0, 0);

	for (uint16_t x = 0; x < screenSize.x; x++) {
		for (uint16_t y = 0; y < screenSize.y; y++) {
			visionImage.setPixel(x, y, clearColor);
		}
	}
	
	float coneAngleRadians = coneAngleDegrees * Mathf::PI / 180.f;

	memoryBlur();
	visionUpdate(posX, posY, rotation - (coneAngleRadians / 2.f), rayCountTotal, coneAngleRadians);
	visionMemorize();

	return visionImage;
}
sf::Image& VisionRenderer::visionProcess(sf::Vector2f pos, float rotation, uint32_t rayCountTotal, float coneAngleDegrees) {
	return visionProcess(pos.x, pos.y, rotation, rayCountTotal, coneAngleDegrees);
}

const sf::Texture& VisionRenderer::memoryGet() {
	return memoryTexture.getTexture();
}

void VisionRenderer::visionUpdate(float posX, float posY, float rotation, uint32_t rayCountTotal, float coneAngleRadians) {

	auto& worldGrid = GameLevelGrid::levelGet(0, 0, 0)->worldGrid;

	const sf::Vector2f rayPositionStarting(posX, posY);

	static uint32_t subConeRayAnchorCur = 0;

	auto& worldImage = worldGrid.imageGrid.worldImageFromPixel(0, 0);

	for (uint32_t rayCur = 0; rayCur < rayCountTotal; rayCur++) {

		const float rayRotation = rotation + (rayCur * (coneAngleRadians / rayCountTotal));
		const sf::Vector2f rayHeadingOrig = sf::Vector2f(cos(rayRotation), sin(rayRotation));
		
		sf::Vector2f rayHeading = rayHeadingOrig;
		sf::Vector2f rayPosition = rayPositionStarting;
	
		float dist = 0;
		constexpr float desiredDist = 500;

		while (dist <= desiredDist) {


			worldGrid.distortionGrid.headingApplyDistortionSafe(rayHeading, uint32_t(rayPosition.x), uint32_t(rayPosition.y));
			rayPosition += rayHeading;
			dist += 1.f;

			if (rayPosition.x < 0 || rayPosition.x >= screenSize.x || rayPosition.y < 0 || rayPosition.y >= screenSize.y) break;

			sf::Color worldColor = worldImage.getPixel(uint16_t(rayPosition.x), uint16_t(rayPosition.y));

			const sf::Vector2f rayVisionPos = rayPositionStarting + (rayHeadingOrig * dist);

			if (!(rayVisionPos.x < 0 || rayVisionPos.x >= screenSize.x || rayVisionPos.y < 0 || rayVisionPos.y >= screenSize.y)) {
				visionImage.setPixel(uint16_t(rayVisionPos.x), uint16_t(rayVisionPos.y), worldColor);
			}
		}
	}
}

void VisionRenderer::visionMemorize() {
	sf::Texture visionTexture;
	visionTexture.create(screenSize.x, screenSize.y);
	visionTexture.loadFromImage(visionImage);

	sf::Sprite visionSprite;
	visionSprite.setTexture(visionTexture);

	memoryTexture.draw(visionSprite);
	memoryTexture.display();
}

void VisionRenderer::memoryBlur() {
	sf::Sprite memorySprite;
	memorySprite.setTexture(memoryTexture.getTexture());

	sf::Shader blurShader;
	blurShader.loadFromFile("Include/Shaders/Blur.glsl", sf::Shader::Fragment);
	blurShader.setUniform("dir", sf::Glsl::Vec2(1, 1));
	blurShader.setUniform("blur", 2048.f);

	memoryTexture.draw(memorySprite, &blurShader);
	memoryTexture.display();
}

