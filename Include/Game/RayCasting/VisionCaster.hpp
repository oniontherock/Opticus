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

	const sf::Image& visionImageGet();
	const sf::RenderTexture& renderTextureGet();

protected:
	
	sf::Image visionImage;
	sf::RenderTexture memoryTexture;

	void raysCast(float angleTo, float coneSize, uint32_t rayCount) override;
	void memoryUpdate();
	void memoryBlur();
};

#endif