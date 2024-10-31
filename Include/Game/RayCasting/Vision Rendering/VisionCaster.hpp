#ifndef __VISION_CASTER_H__
#define __VISION_CASTER_H__

#include "../../../Common/Cooldown.hpp"
#include "../../../Common/Math.hpp"
#include "../../../Common/TimeHandler.hpp"
#include "../ACECS/GameLevel.hpp"
#include "../ACECS/Panels.hpp"
#include "../RayCaster.hpp"
#include <cstdint>
#include <SFML/Graphics.hpp>

// casts a series of rays and records the colors they land on at each step.
struct VisionCaster : public RayCaster {

	VisionCaster();
	VisionCaster(sf::Vector2f _castPosition);
	VisionCaster(const VisionCaster& other);
	void operator= (const VisionCaster& other);


	void update(float fromX, float fromY, float angleTo, float coneSize, float rayMaxDist, uint32_t rayCount) override;

	const sf::RenderTexture& visionTextureGet();

protected:
	
	sf::RenderTexture visionTexture;

	void raysCast(float angleTo, float coneSize, float rayMaxDist, uint32_t rayCount) override;
};

#endif