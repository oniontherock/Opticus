#ifndef __DISTORTION_FUNCTIONS_H__
#define __DISTORTION_FUNCTIONS_H__

#include "../../Common/VectorMath.hpp"
#include <functional>

typedef std::function<void(sf::Vector2f&)> DistortionFunction;
extern std::vector<DistortionFunction> distortionFunctions;

enum DistortionFunctionId : uint16_t {
	Slow,
	Stop,
	Accelerate,
	Mirror,
	ENUM_SIZE,
};

void distortionFunctionsInitialize();

struct WorldDistortion {

	std::vector<DistortionFunctionId> distortionFunctionIds;

	WorldDistortion();

	void headingApplyDistortion(sf::Vector2f& heading);
};

#endif