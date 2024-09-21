#ifndef __WORLD_DISTORTION_H__
#define __WORLD_DISTORTION_H__

#include "../../../Common/VectorMath.hpp"
#include "../../../Common/Cooldown.hpp"
#include "../../../Common/TimeHandler.hpp"
#include <functional>

// distorts a heading and a position, can have a capture list for special behaviors
typedef std::function<void(sf::Vector2f& heading, sf::Vector2f& position)> DistortionFunction;
// cooldown on a distortion, when this runs out, the associated DistortionFunction is removed
typedef Cooldown DistortionCooldown;
// pair of DistortionFunction and DistortionCooldown
// When the cooldown is over, the pair should be removed from wherever it is being stored
typedef std::pair<DistortionFunction, DistortionCooldown> Distortion;



struct WorldDistortion {

	std::vector<Distortion> distortions;

	WorldDistortion();
	template <typename... Args>
	WorldDistortion(Args... args) {
		distortions = std::vector<Distortion>{ args... };
	}
	void headingApplyDistortion(sf::Vector2f& heading, sf::Vector2f& position);
	void distortionAdd(Distortion distortion);

private:
	uint32_t lastUpdateTime = TimeHandler::timeSimulatedGet();
};

#endif