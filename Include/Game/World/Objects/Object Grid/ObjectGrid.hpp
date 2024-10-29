#ifndef __OBJECT_GRID_H__
#define __OBJECT_GRID_H__

#include "ObjectCell.hpp"
#include <vector>
#include "../../../../Common/Grid.hpp"

typedef std::vector<ObjectCell> ObjectGrid1D;
typedef std::vector<ObjectGrid1D> ObjectGrid2D;

struct ObjectGrid : Grid<ObjectCell> {
	ObjectGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);
};

#endif