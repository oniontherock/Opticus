#ifndef __MEMORY_HOLDER_VISION_H__
#define __MEMORY_HOLDER_VISION_H__

#include "../../../Common/Cooldown.hpp"
#include "../../../Common/TimeHandler.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <World/LevelTypeDefinitions.hpp>

// records textures passed to it to four textures that will move to create a seamless effect
struct MemoryHolderVision {

	MemoryHolderVision();
	MemoryHolderVision(const MemoryHolderVision& other);
	void operator= (const MemoryHolderVision& other);

	// center of the four memory textures
	sf::Vector2f positionCenter;

	// offset for the four textures from the center
	sf::Vector2f offsetCenter;

	sf::RenderTexture& textureMemoryGet();

	// update the offsetCenter with offsetX and offsetY
	void offsetCenterUpdate(float offsetX, float offsetY);
	// update the memory textures with textureUpdate
	void memoryUpdate(sf::Texture& textureUpdate);

protected:

	// size of each textureMemory
	sf::Vector2i texturesMemorySizeEach;

	std::vector<sf::RenderTexture> texturesMemoryVector;

	Cooldown blurCooldown = Cooldown(0.1f);

	// concatenation of the texturesMemoryVector
	sf::RenderTexture textureMemoryConcatenated;

	void memoryBlur();
	void textureMemoryConcatenatedUpdate();
};


#endif