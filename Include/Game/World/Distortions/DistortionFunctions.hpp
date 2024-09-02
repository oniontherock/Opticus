#ifndef __DISTORTION_FUNCTIONS_H__
#define __DISTORTION_FUNCTIONS_H__

#include "../../../Common/VectorMath.hpp"
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

	std::vector<uint16_t> distortionFunctionIds;

	WorldDistortion();
	template <typename... Args>
	WorldDistortion(Args... args) {
		distortionFunctionIds = std::vector<uint16_t>{ args... };
	}

	void headingApplyDistortion(sf::Vector2f& heading);
};

#endif