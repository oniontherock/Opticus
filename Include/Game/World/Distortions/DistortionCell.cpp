#include "DistortionCell.hpp"
#include "DistortionIds.hpp"

DistortionCell::DistortionCell() {

	using namespace Distortions;

	std::vector<std::vector<DistortionSharedPtr>> rows(totalDistortions);

	for (uint32_t x = 0; x < totalDistortions; x++) {

		std::vector<DistortionSharedPtr> columns = std::vector<DistortionSharedPtr>(0);

		rows[x] = std::move(columns);
	}

	distortions = std::move(rows);
}

Distortions::Distortion* DistortionCell::distortionGet(Distortions::DistortionTypeId id, uint16_t ind) {
	return distortions[id][ind].get();
}
void DistortionCell::distortionTerminate(Distortions::DistortionTypeId id, uint16_t ind) {
	distortions[id].erase(distortions[id].begin() + ind);
}
void DistortionCell::distortionTerminateAllOfType(Distortions::DistortionTypeId id) {
	for (int32_t i = int32_t(distortions[id].size() - 1); i >= 0; i--) {
		distortionTerminate(id, i);
	}
}

