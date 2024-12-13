#ifndef __DISTORTION_H__
#define __DISTORTION_H__

#include <Auxiliary/VectorMath.hpp>
#include <Auxiliary/Cooldown.hpp>
#include <queue>
#include <Auxiliary/TimeHandler.hpp>
#include <functional>

enum class DistortionType {
	HeadingMultiplier,
	HeadingOffset,
	PositionOffset,
};

// offset for a ray's position or heading
typedef sf::Vector2f DistortionOffset;
// pair of DistortionType and DistortionOffset
typedef std::pair<DistortionType, DistortionOffset> DistortionTypeOffset;
// time in milliseconds since epoch at which the distortion is removed
typedef uint32_t DistortionDeathTime;
// time in milliseconds that a distortion lasts, usually gets converted into a DistortionDeathTime
typedef uint32_t DistortionLength;

// contains a DistortionDeathTime, and a DistortionTypeOffset
struct DistortionValue {
	DistortionValue(DistortionType type, DistortionOffset offset, DistortionLength length);

	DistortionTypeOffset typeOffset;
	DistortionDeathTime deathTime;

};
bool operator< (const DistortionValue& l, const DistortionValue& r);
bool operator> (const DistortionValue& l, const DistortionValue& r);
bool operator<= (const DistortionValue& l, const DistortionValue& r);
bool operator>= (const DistortionValue& l, const DistortionValue& r);
bool operator== (const DistortionValue& l, const DistortionValue& r);


class Distortion {

	// multiplier for heading
	DistortionOffset headingMultiplier = DistortionOffset(1.f, 1.f);
	// offset for heading
	DistortionOffset headingOffset;

	// offset for position
	DistortionOffset positionOffset;

	// updates the distortionValues priority_queue (I.E. checks if any distortions need to be removed
	void distortionValuesUpdate();
	// removes the top DistortionValue from the distortionValues priority_queue, and undoes it's DistortionOffsets
	void distortionValueRemove();
	
	// modifies the DistortionOffsets by the typeOffset
	void distortionTypeOffsetsModify(DistortionType type, DistortionOffset offset);
	// undoes a DistortionTypeOffset
	void distortionTypeOffsetUndo(DistortionType type, DistortionOffset offset);
	// applies the DistortionType and offset to the Distortion's DistortionOffset
	void distortionTypeOffsetsApply(DistortionType type, DistortionOffset offset);

	std::priority_queue<DistortionValue, std::vector<DistortionValue>, std::greater<DistortionValue>> distortionValues;

	// applies the various offsets to a ray's heading and position
	inline void distortionOffsetRayValues(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) {
		// apply headingMultiplier
		rayHeading.x *= headingMultiplier.x;
		rayHeading.y *= headingMultiplier.y;
		// apply headingOffset
		rayHeading += headingOffset;

		// apply positionOffset
		rayPosition += positionOffset;
	}

public:

	void distortionAdd(DistortionType type, DistortionOffset offset, DistortionLength length);

	// updates the distortionValues priority_queue and offsets a ray's values
	inline void distortionApplyToRay(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) {
		if (distortionValues.empty()) return;

		distortionValuesUpdate();
		distortionOffsetRayValues(rayHeading, rayPosition);
	}
};

#endif