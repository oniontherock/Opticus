#ifndef __MEMORY_HOLDER_VISION_H__
#define __MEMORY_HOLDER_VISION_H__

#include "../../../Common/Cooldown.hpp"
#include "../../../Common/TimeHandler.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <World/LevelTypeDefinitions.hpp>

// records the texture passed to it in the memoryUpdate function,
// and scrolls based off of the memoryMovement parameter of the memoryUpdate function
struct MemoryHolderVision {

	MemoryHolderVision();
	MemoryHolderVision(sf::Vector2f _memorySize);
	MemoryHolderVision(const MemoryHolderVision& other);
	void operator= (const MemoryHolderVision& other);
	
	// size of the textureMemory in pixels
	sf::Vector2f memorySize;

	// updates the textureMemory by scrolling it based off of memoryMovement,
	// then drawing a grayscaled version of the textureVision,
	// and finally blurring the textureMemory.
	void memoryUpdate(sf::Vector2f memoryMovement, sf::Texture textureVision);
	// returns a reference to the textureMemory
	sf::RenderTexture& memoryGet();

protected:

	// the texture used to record memories
	sf::RenderTexture textureMemory;

	// the total amount of memoryMovement since last scroll,
	sf::Vector2f memoryMovementTotal;
	// the amount of memoryMovement needed to actually update the scroll,
	// this has to be at least 1, because the memory cannot be scrolled by anything less than 1, since pixels can only be moved by integers.
	const float memoryMovementStep = 1.f;

	//// noise texture used by spriteNoise
	//sf::Texture textureNoise;
	//// noise sprite used for clearing textureMemory
	//sf::Sprite spriteNoise;

	//void initializeNoise();

	// cooldown between blurs
	Cooldown blurCooldown = Cooldown(0.1f);
	uint16_t timeOfLastBlur;

	// blurs the memory
	void memoryBlur();

};


#endif