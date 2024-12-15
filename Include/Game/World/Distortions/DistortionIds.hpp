#ifndef __DISTORTION_IDS_H__
#define __DISTORTION_IDS_H__

#include "Distortion.hpp"
#include <cstdint>
#include <vector>

namespace Distortions {

	// an ID representing a certain TYPE of distortion, NOT a specific distortion instance
	typedef uint16_t DistortionTypeId;

	// the maximum value for any ID
	inline DistortionTypeId maxID = 0;
	// the total amount of distortions
	inline DistortionTypeId totalDistortions = 0;
	// a vector of instances of every type of distortion, sorted by distortion type. Assigned when the distortion's Id is registered.
	inline std::vector<DistortionUniquePtr> distortionsAll;

	// DistortionTypesHolder is a sort of helper struct that holds the DistortionTypeID of every type of distortion,
	// an ID is simply an integer assigned uniquely to every child of base class "Distortion",
	// the way you get the ID of a distortion type is like this: "DistortionIDHolder<TYPE>::ID".
	template <class T>
	struct DistortionIds {
		static DistortionTypeId ID;

		static void IDSet(DistortionTypeId value) {
			ID = value;

			if (ID > maxID) maxID = ID;

			if (ID + 1 > totalDistortions) totalDistortions = ID + 1;

			distortionsAll.push_back(DistortionUniquePtr(new T()));
		}
	};

	// define DistortionIDHolder's ID variable as 0 for every type.
	// note that the ID for every type is assigned later, 
	// if you wish to see how this is done, see the "DistortionRegistry.cpp" file, or the "TypeAllocator.hpp" file.

	// the ID of a distortion type, used for accessing certain types of distortions in arrays, among other things
	template <class T>
	DistortionTypeId DistortionIds<T>::ID{ 0 };
};

#endif