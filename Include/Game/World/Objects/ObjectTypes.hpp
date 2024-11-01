#ifndef __OBJECT_TYPES_H__
#define __OBJECT_TYPES_H__

#include <cstdint>

enum class ObjectType : uint16_t {
	Null, // no object type
	Player,
	SquadMember1, // squad member 1, A.K.A. Bravo
	SquadMember2, // squad member 2, A.K.A. Charlie
	SquadMember3, // squad member 3, A.K.A. Delta
	Door,
	Skipper,
	Wall,
	SIZE, // size of the ObjectTypes enum
};



#endif