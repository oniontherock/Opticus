#include "DistortionFunctions.hpp"


std::vector<DistortionFunction> distortionFunctions{};

void distortionFunctionsInitialize() {

	distortionFunctions.resize(DistortionFunctionId::ENUM_SIZE);

	distortionFunctions[Slow] = [](sf::Vector2f& heading) {
		heading /= 1.0025f;
		};
	distortionFunctions[Stop] = [](sf::Vector2f& heading) {
		heading *= 999999999.f;
		};
	distortionFunctions[Accelerate] = [](sf::Vector2f& heading) {
		heading *= 1.001f;
		};
	distortionFunctions[Mirror] = [](sf::Vector2f& heading) {
		heading = Vector2fMath::reflect(heading, sf::Vector2f(1, 0));
		};
}

WorldDistortion::WorldDistortion() {
	distortionFunctionIds = std::vector<uint16_t>{ };
}

#include <iostream>

void WorldDistortion::headingApplyDistortion(sf::Vector2f& heading) {
	for (uint16_t i = 0; i < distortionFunctionIds.size(); i++) {
		std::invoke(distortionFunctions[distortionFunctionIds[i]], heading);
	}
}


