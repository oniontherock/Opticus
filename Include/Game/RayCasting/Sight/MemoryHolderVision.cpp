#include "MemoryHolderVision.hpp"


MemoryHolderVision::MemoryHolderVision() {
	positionCenter = sf::Vector2f(0.f, 0.f);
	offsetCenter = sf::Vector2f(0.f, 0.f);

	texturesMemorySizeEach = sf::Vector2i(500.f, 500.f);

	texturesMemoryVector = std::vector<sf::RenderTexture>(4);
	for (uint8_t i = 0; i < 4; i++) {
		texturesMemoryVector[i].create(texturesMemorySizeEach.x, texturesMemorySizeEach.y);
	}

	textureMemoryConcatenated.create(texturesMemorySizeEach.x * 2.f, texturesMemorySizeEach.y * 2.f);
}

MemoryHolderVision::MemoryHolderVision(const MemoryHolderVision& other) {
	positionCenter = other.positionCenter;
	offsetCenter = other.offsetCenter;

	texturesMemorySizeEach = other.texturesMemorySizeEach;

	texturesMemoryVector = std::vector<sf::RenderTexture>(4);
	for (uint8_t i = 0; i < 4; i++) {
		texturesMemoryVector[i].create(texturesMemorySizeEach.x, texturesMemorySizeEach.y);
	}

	textureMemoryConcatenated.create(texturesMemorySizeEach.x * 2.f, texturesMemorySizeEach.y * 2.f);
}
void MemoryHolderVision::operator= (const MemoryHolderVision& other) {
	positionCenter = other.positionCenter;
	offsetCenter = other.offsetCenter;

	texturesMemorySizeEach = other.texturesMemorySizeEach;

	texturesMemoryVector = std::vector<sf::RenderTexture>(4);
	for (uint8_t i = 0; i < 4; i++) {
		texturesMemoryVector[i].create(texturesMemorySizeEach.x, texturesMemorySizeEach.y);
	}

	textureMemoryConcatenated.create(texturesMemorySizeEach.x * 2.f, texturesMemorySizeEach.y * 2.f);
}

sf::RenderTexture& MemoryHolderVision::textureMemoryGet() {
	return textureMemoryConcatenated;
}

void MemoryHolderVision::offsetCenterUpdate(float offsetX, float offsetY) {
	offsetCenter.x += offsetX;
	offsetCenter.y += offsetY;
}

void MemoryHolderVision::memoryUpdate(const sf::Texture& textureUpdate) {
	// blur the memory
	memoryBlur();

	for (uint8_t i = 0; i < 4; i++) {
		sf::RenderTexture& textureMemory = texturesMemoryVector[i];

		// create a sprite from the texture
		sf::Sprite spriteUpdate(textureUpdate);
		spriteUpdate.setOrigin(sf::Vector2f(textureUpdate.getSize()) / 2.f);
		// set the spriteUpdate's position to the center of the textureMemory, minus the offsetCenter
		spriteUpdate.setPosition((sf::Vector2f(textureMemory.getSize()) / 2.f) - offsetCenter);

		// load the grayscale shader
		sf::Shader grayscaleShader;
		grayscaleShader.loadFromFile("Include/Shaders/Grayscale.glsl", sf::Shader::Fragment);
		grayscaleShader.setUniform("divider", 2.0f);
		grayscaleShader.setUniform("lerp", 0.9f);


		// draw the spriteUpdate to the memoryTexture with a grayscale applied
		textureMemory.draw(spriteUpdate, &grayscaleShader);
		// display the memoryTexture
		textureMemory.display();
	}

	textureMemoryConcatenatedUpdate();
}

void MemoryHolderVision::memoryBlur() {

	// get simulated delta
	float delta = float(TimeHandler::deltaSimulatedGet());

	// update the blurCooldown, if it is ready, blur the memoryTexture a bit
	if (blurCooldown.updateAutoReset(delta)) {
	
		for (uint8_t i = 0; i < 4; i++) {

			sf::RenderTexture& textureMemory = texturesMemoryVector[i];

			// create a sprite from the memoryTexture
			sf::Sprite memorySprite(textureMemory.getTexture());

			// load the blur shader
			sf::Shader blurShader;
			blurShader.loadFromFile("Include/Shaders/Blur.glsl", sf::Shader::Fragment);
			blurShader.setUniform("directions", 4.f);
			blurShader.setUniform("quality", 8.f);
			blurShader.setUniform("size", 1.f);

			// draw to the memoryTexture with the memorySprite with the blur shader applied, effectively blurring the memory.
			textureMemory.draw(memorySprite, &blurShader);
			// display the memoryTexture
			textureMemory.display();
		}
	}
}

void MemoryHolderVision::textureMemoryConcatenatedUpdate() {

	sf::Sprite sprite0(texturesMemoryVector[0].getTexture());

	sf::Sprite sprite1(texturesMemoryVector[1].getTexture());
	sprite1.setPosition(texturesMemorySizeEach.x, 0.f);

	sf::Sprite sprite2(texturesMemoryVector[2].getTexture());
	sprite2.setPosition(0.f, texturesMemorySizeEach.y);

	sf::Sprite sprite3(texturesMemoryVector[3].getTexture());
	sprite3.setPosition(texturesMemorySizeEach.x, texturesMemorySizeEach.y);


	textureMemoryConcatenated.clear();
	textureMemoryConcatenated.draw(sprite0);
	textureMemoryConcatenated.draw(sprite1);
	textureMemoryConcatenated.draw(sprite2);
	textureMemoryConcatenated.draw(sprite3);
	textureMemoryConcatenated.display();
}



