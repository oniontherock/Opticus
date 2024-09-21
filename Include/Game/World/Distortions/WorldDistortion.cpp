#include "WorldDistortion.hpp"
#include <iostream>

WorldDistortion::WorldDistortion() {
	distortions = std::vector<Distortion>();
}

void WorldDistortion::headingApplyDistortion(sf::Vector2f& heading, sf::Vector2f& position) {

	const uint32_t curTime = TimeHandler::timeSimulatedGet();

	const float timeDiffSeconds = float(curTime - lastUpdateTime) / 1000.f;

	for (int32_t i = static_cast<int32_t>(distortions.size()) - 1; i >= 0; i--) {
		if (distortions[i].second.updateAutoReset(timeDiffSeconds)) {
			distortions.erase(distortions.begin() + i);
		}
		else {
			std::invoke(distortions[i].first, heading, position);
		}
	}

	lastUpdateTime = curTime;
}
void WorldDistortion::distortionAdd(Distortion distortion) {
	distortions.push_back(distortion);
}

