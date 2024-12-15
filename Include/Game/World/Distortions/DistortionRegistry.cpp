#include "DistortionRegistry.hpp"
#include <ECS/TypeIDAllocator.hpp>
#include "DistortionIds.hpp"

using namespace Distortions;

void Distortions::distortionsRegister() {
	using DistortionRegistry = TypeIDAllocator<Distortion>;

	DistortionRegistry::typeRegister<DistortionIds<DistortionPositionSet>>();
	DistortionRegistry::typeRegister<DistortionIds<DistortionHeadingMultiply>>();
}

void DistortionHeadingMultiply::distortionApplyToRay(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) {
	rayHeading *= multiplier;
}

void DistortionPositionSet::distortionApplyToRay(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) {
	rayPosition = point;
}



