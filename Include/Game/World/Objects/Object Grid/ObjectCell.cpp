#include "ObjectCell.hpp"
#include <cstdint>


ObjectCell::ObjectCell() {
	updateTimeLast = TimeHandler::timeSimulatedGet();
}

void ObjectCell::idAdd(IdDurationPair idDurationPair) {
	IdsVector.push_back(idDurationPair);
}
void ObjectCell::idAdd(EntityId id, IdDuration duration) {
	idAdd(IdDurationPair(id, duration));
}
void ObjectCell::idAdd(EntityId id, float duration) {
	idAdd(id, IdDuration(duration));
}

const std::vector<EntityId> ObjectCell::idsGet() {
	std::vector<EntityId> entityIds;

	for (uint16_t i = 0; i < IdsVector.size(); i++) {
		entityIds.push_back(IdsVector[i].first);
	}

	idsVectorUpdate();

	return entityIds;
}

void ObjectCell::idsVectorUpdate() {
	const uint32_t curTime = TimeHandler::timeSimulatedGet();

	const float timeDiffSeconds = float(curTime - updateTimeLast) / 1000.f;

	for (int32_t i = static_cast<int32_t>(IdsVector.size()) - 1; i >= 0; i--) {
		if (IdsVector[i].second.updateAutoReset(timeDiffSeconds)) {
			IdsVector.erase(IdsVector.begin() + i);
		}
	}

	updateTimeLast = curTime;
}
