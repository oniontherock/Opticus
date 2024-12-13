#include "Distortion.hpp"
#include <Auxiliary/TimeHandler.hpp>


DistortionValue::DistortionValue(DistortionType type, DistortionOffset offset, DistortionLength length) {
	typeOffset = DistortionTypeOffset(type, offset);
	deathTime = TimeHandler::timeSimulatedGet() + length;
}

bool operator< (const DistortionValue& l, const DistortionValue& r) {
	return l.deathTime < r.deathTime;
}
bool operator> (const DistortionValue& l, const DistortionValue& r) {
	return r < l;
}
bool operator<= (const DistortionValue& l, const DistortionValue& r) {
	return !(l > r);
}
bool operator>= (const DistortionValue& l, const DistortionValue& r) {
	return !(l < r);
}
bool operator== (const DistortionValue& l, const DistortionValue& r) {
	return l.deathTime == r.deathTime;
}

void Distortion::distortionAdd(DistortionType type, DistortionOffset offset, DistortionLength length) {
	distortionValues.push(DistortionValue(type, offset, length));
	distortionTypeOffsetsApply(type, offset);
}

void Distortion::distortionValuesUpdate() {
	uint32_t time = TimeHandler::timeSimulatedGet();

	while ((distortionValues.top().deathTime < time) && (!distortionValues.empty())) {
		distortionValueRemove();
	}
}

void Distortion::distortionValueRemove() {
	// get top value
	DistortionValue value = distortionValues.top();
	// remove top value from distortionValues priority_queue
	distortionValues.pop();
	// undo the DistortionTypeOffset of the DistortionValue
	distortionTypeOffsetUndo(value.typeOffset.first, value.typeOffset.second);
}

void Distortion::distortionTypeOffsetsModify(DistortionType type, DistortionOffset offset) {
	switch (type) {
	case DistortionType::HeadingMultiplier:
		headingMultiplier += offset;
		break;
	case DistortionType::HeadingOffset:
		headingOffset += offset;
		break;
	case DistortionType::PositionOffset:
		positionOffset += offset;
		break;
	}
}


void Distortion::distortionTypeOffsetUndo(DistortionType type, DistortionOffset offset) {
	distortionTypeOffsetsModify(type, -offset);
}

void Distortion::distortionTypeOffsetsApply(DistortionType type, DistortionOffset offset) {
	distortionTypeOffsetsModify(type, offset);
}

