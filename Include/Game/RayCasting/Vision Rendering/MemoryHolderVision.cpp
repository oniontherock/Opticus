#include "MemoryHolderVision.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <Auxiliary/Math.hpp>

MemoryHolderVision::MemoryHolderVision() {
	memorySize = sf::Vector2f(1, 1);
	textureMemory.create(uint32_t(memorySize.x), uint32_t(memorySize.y));

	//initializeNoise();
}
MemoryHolderVision::MemoryHolderVision(sf::Vector2f _memorySize) {
	memorySize = _memorySize;
	textureMemory.create(uint32_t(memorySize.x), uint32_t(memorySize.y));

	//initializeNoise();
}
MemoryHolderVision::MemoryHolderVision(const MemoryHolderVision& other) {
	memorySize = other.memorySize;
	textureMemory.create(uint32_t(memorySize.x), uint32_t(memorySize.y));

	//initializeNoise();
}
void MemoryHolderVision::operator= (const MemoryHolderVision& other) {
	memorySize = other.memorySize;
	textureMemory.create(uint32_t(memorySize.x), uint32_t(memorySize.y));

	//initializeNoise();
}
//
//void MemoryHolderVision::initializeNoise() {
//	textureNoise.loadFromFile("Art/Cobweb Noise.jpg");
//	textureNoise.setRepeated(true);
//
//	spriteNoise.setTexture(textureNoise, true);
//	spriteNoise.setColor(sf::Color(130, 130, 130, 255));
//}


void MemoryHolderVision::memoryUpdate(sf::Vector2f memoryMovement, sf::Texture textureVision) {
	
	// get a copy of the textureMemory's texture, as it is going to be cleared before this texture is used.
	sf::Texture textureMemoryCopy = textureMemory.getTexture();
	// create a sprite from the textureMemoryCopy
	sf::Sprite memorySprite(textureMemoryCopy);

	// add to the memoryMovementTotal by the memoryMovement.
	// 
	// this is done because the memory cannot be moved by values less than 1,
	// so we tally up the movement until it is higher than memoryMovementStep (which is usually 1), and then apply it.
	memoryMovementTotal += memoryMovement;

	// check if the memoryMovementTotal on the X axis is more than the memoryMovementStep (which is usually 1),
	// and if it is, move by the step, and subtract the step from the memoryMovementTotal
	while (abs(memoryMovementTotal.x) > memoryMovementStep) {
		memorySprite.move(memoryMovementStep * Mathf::sign(memoryMovementTotal.x), 0.f);
		// less than 0, step up
		if (Mathf::sign(memoryMovementTotal.x) < 0) memoryMovementTotal.x += memoryMovementStep;
		// higher than 0, step down
		else memoryMovementTotal.x -= memoryMovementStep;
	}
	// check if the memoryMovementTotal on the Y axis is more than the memoryMovementStep (which is usually 1),
	// and if it is, move by the step, and subtract the step from the memoryMovementTotal
	while (abs(memoryMovementTotal.y) > memoryMovementStep) {
		memorySprite.move(0.f, memoryMovementStep * Mathf::sign(memoryMovementTotal.y));
		// less than 0, step up
		if (Mathf::sign(memoryMovementTotal.y) < 0) memoryMovementTotal.y += memoryMovementStep;
		// higher than 0, step down
		else memoryMovementTotal.y -= memoryMovementStep;
	}

	// clear the textureMemory
	textureMemory.clear();
	// draw to the textureMemory with the memorySprite, which was moved by memoryMovement.
	textureMemory.draw(memorySprite);

	// load the grayscale shader
	sf::Shader grayscaleShader;
	grayscaleShader.loadFromFile("Include/Shaders/Grayscale.glsl", sf::Shader::Fragment);
	grayscaleShader.setUniform("divider", 2.0f);
	grayscaleShader.setUniform("lerp", 0.9f);

	// draw vision to memory
	sf::Sprite visionSprite(textureVision);
	visionSprite.setOrigin(sf::Vector2f(textureVision.getSize()) / 2.f);
	visionSprite.setPosition(sf::Vector2f(textureMemory.getSize()) / 2.f);

	// draw the visionSprite with the grayscaleShader applied
	textureMemory.draw(visionSprite, &grayscaleShader);
	// display the textureMemory
	textureMemory.display();

	// blur the textureMemory
	memoryBlur();
}
sf::RenderTexture& MemoryHolderVision::memoryGet() {
	return textureMemory;
}
void MemoryHolderVision::memoryBlur() {
	// get simulated delta
	float delta = float(TimeHandler::timeSimulatedGet() - timeOfLastBlur) / 1000.f;

	// update the blurCooldown, if it is ready, blur the memoryTexture a bit
	if (blurCooldown.updateAutoReset(delta)) {

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
	timeOfLastBlur = TimeHandler::timeSimulatedGet();
}
