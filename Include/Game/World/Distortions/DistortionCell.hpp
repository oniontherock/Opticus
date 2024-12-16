#ifndef __DISTORTION_CELL_H__
#define __DISTORTION_CELL_H__

#include "Distortion.hpp"
#include "DistortionIds.hpp"
#include <Auxiliary/TimeHandler.hpp>
#include <vector>


struct DistortionCell {

	DistortionCell();

	std::vector<std::vector<Distortions::DistortionSharedPtr>> distortions;

	Distortions::Distortion* distortionGet(Distortions::DistortionTypeId id, uint16_t ind);
	template <class T>
	Distortions::Distortion* distortionGet(uint16_t ind) {
		return distortionGet(Distortions::DistortionIds<T>::ID, ind);
	}
	template <class T>
	T* distortionGet() {
		return distortionGet<T>(0);
	}
	template <class T>
	bool distortionHas() {
		return distortions[Distortions::DistortionIds<T>::ID].size() > 0;
	}
	// adds the specified distortion to the distortions vector, with the lifetime of "distortionLength" (distortionLength is in milliseconds)
	template <class T>
	void distortionAdd(Distortions::DistortionSharedPtr distortion, uint32_t distortionLength) {
		distortion->deathTime = TimeHandler::timeSimulatedGet() + distortionLength;

		distortions[Distortions::DistortionIds<T>::ID].push_back(distortion);
	}

	void distortionTerminate(Distortions::DistortionTypeId id, uint16_t ind);

	void distortionTerminateAllOfType(Distortions::DistortionTypeId id);
	template <class T>
	void distortionTerminateAllOfType() {
		distortionTerminateAllOfType(Distortions::DistortionIds<T>::ID);
	}


	// checks if a distortion should be removed (if it's deathtime has been reached), if it is removed, returns false 
	inline bool distortionValidateLife(Distortions::DistortionTypeId id, uint16_t ind) {
		if (TimeHandler::timeSimulatedGet() > uint32_t(distortionGet(id, ind)->deathTime)) {
			distortionTerminate(id, ind);
			return false;
		}
		return true;
	}


	inline void distortionsApplyToRay(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) {
		for (Distortions::DistortionTypeId id = 0; id < Distortions::totalDistortions; id++) {
			// iterate backwards because the function "distortionValidateLife" may remove distortions
			for (int32_t ind = int32_t(distortions[id].size() - 1); ind >= 0; ind--) {
				if (distortionValidateLife(id, ind)) {
					distortionGet(id, ind)->distortionApplyToRay(rayHeading, rayPosition);
				}
			}
		}
	}
};


#endif