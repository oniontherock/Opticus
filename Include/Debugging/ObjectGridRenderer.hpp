#ifndef __OBJECT_GRID_RENDERER_H__
#define __OBJECT_GRID_RENDERER_H__

#include "../ACECS/GameLevel.hpp"
#include "../ACECS/Panels.hpp"
#include "../Game/World/Objects/Object Grid/ObjectGrid.hpp"
#include "../Game/World/Objects/ObjectRegistry.hpp"
#include "../Game/World/Objects/ObjectTypes.hpp"

class ObjectGridRenderer {
public:
	ObjectGridRenderer();

	void objectGridRender(ObjectGrid& objectGrid);

	const sf::Texture& objectGridTextureGet() const;

private:
	sf::Image objectGridImage;
	sf::Texture objectGridTexture;

	// the current cycle in the rendering process.
	// chunks of the objectGrid are rendered one at a time to increase performance,
	// the current chunk is dependent on the renderCycle in the X and Y axis
	sf::Vector2i renderCycle = sf::Vector2i(1, 1);
	// max renderCycle for each axis before incrementing.
	// if X reaches this: X=1, Y++
	// if Y reaches this: Y=1
	constexpr static uint16_t renderCycleMax = 4;

	void renderCycleUpdate();

	std::vector<sf::Color> objectTypeColors;
};


#endif