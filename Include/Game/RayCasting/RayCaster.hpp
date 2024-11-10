#ifndef __RAY_CASTER_H__
#define __RAY_CASTER_H__

#include <cstdint>
#include <World/LevelTypeDefinitions.hpp>
#include <SFML/System/Vector2.hpp>

// casts a series of rays out from a position
struct RayCaster {
	// update the RayCaster, calls raysCast().
	// also updates castPosition with fromX and fromY
	virtual void update(float fromX, float fromY, float toAngle, float coneSize, float rayMaxDist, uint32_t rayCount);

protected:

	// the position to cast the rays from.
	// used in raysCast for where to cast rays from,
	// can also be used by derived classes for other things.
	WorldPosition castPosition;
	float castAngle;

	/**
		casts out a series of rays from a specified position towards an angle in a cone
	
		@param fromX: the x position to cast from (in room coordinates).
		@param fromY: the y position to cast from (in room coordinates).
		@param angleTo: the angle to cast towards.
		@param coneSize: the angular size of the cone to cast rays in.
		@param rayCount: the amount of rays to cast.
	*/
	virtual void raysCast(float coneSize, float rayMaxDist, uint32_t rayCount) = 0;
};



#endif