#ifndef __WORLD_IMAGE_GRID_H__
#define __WORLD_IMAGE_GRID_H__

#include "SFML/Graphics.hpp"
#include "WorldTexture.hpp"
#include "../../../Common/Grid.hpp"

typedef std::vector<WorldTexture> WorldImageGrid1D;
typedef std::vector<WorldImageGrid1D> WorldImageGrid2D;

struct WorldImageGrid : Grid<WorldTexture> {

	WorldImageGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	void imagesAllReadSet(const bool state);
	void imagesAllWriteSet(const bool state);
};


#endif
