#ifndef __VISION_CASTER_H__
#define __VISION_CASTER_H__

#include "../ACECS/GameLevel.hpp"
#include "../ACECS/Panels.hpp"
#include "RayCaster.hpp"
#include <cstdint>
#include <SFML/Graphics.hpp>

// casts a series of rays and records the colors they land on at each step.
// also records the previously recorded colors to a memory texture.
struct VisionCaster : public RayCaster {

	VisionCaster();
	VisionCaster(const VisionCaster& other);
	void operator= (const VisionCaster& other);


	void update(float fromX, float fromY, float angleTo, float coneSize, uint32_t rayCount) override;

	const sf::Sprite& visionSpriteGet();
	const sf::Sprite& memorySpriteGet();

protected:
	sf::Image visionImage;
	sf::RenderTexture memoryTexture;

	// a sprite created from visionImage, updated in the spritesUpdate() function.
	// this is only really used in visionSpriteRetrieve, but it's stored in the class so it doesn't have to be created every call of visionSpriteGet.
	sf::Sprite visionSprite;
	// a sprite created from memoryTexture, updated in the spritesUpdate() function.
	// this is only really used in memorySpriteRetrieve, but it's stored in the class so it doesn't have to be created every call of memorySpriteGet.
	sf::Sprite memorySprite;

	// updates visionSprite and memorySprite.
	void spritesUpdate();

	void raysCast(float angleTo, float coneSize, uint32_t rayCount) override;
	void memoryUpdate();
	void memoryBlur();
};

#endif