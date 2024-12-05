#ifndef __VISION_CASTER_H__
#define __VISION_CASTER_H__

#include <Auxiliary/Cooldown.hpp>
#include <Auxiliary/Math.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include "../ACECS/GameLevel.hpp"
#include "../ACECS/Panels.hpp"
#include "../RayCaster.hpp"
#include <cstdint>
#include <SFML/Graphics.hpp>

// casts a series of rays and records the colors they land on at each step.
struct VisionCaster : public RayCaster {

	VisionCaster();
	VisionCaster(sf::Vector2f _castPosition);
	VisionCaster(bool _renderVision, bool _renderAround);
	VisionCaster(const VisionCaster& other);
	void operator= (const VisionCaster& other);


	void visionClear();
	void visionDisplay();

	void update(float fromX, float fromY, float angleTo, float coneSize, float rayMaxDist, uint32_t rayCount) override;

	void textureToSeeSet(sf::RenderTexture& texture);

	const sf::RenderTexture& visionTextureGet();

protected:
	
	sf::RenderTexture visionTexture;
	sf::Image visionImage;
	
	// the center of the visionImage
	sf::Vector2f visionImageCenter;
	// center of the GameView panel's view in global room coordinates
	sf::Vector2f cameraCenterGlobal;
	// center of the GameView panel's view localized around the global cast position,
	sf::Vector2f cameraCenterLocal;

	// texture that the raysCast function will use when casting
	sf::RenderTexture* textureToSee;

	void raysCast(float coneSize, float rayMaxDist, uint32_t rayCount) override;
};

#endif