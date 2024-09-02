#ifndef __WORLD_IMAGE_GRID_H__
#define __WORLD_IMAGE_GRID_H__

#include "SFML/Graphics.hpp"

typedef sf::Image WorldImage;
typedef std::vector<WorldImage> WorldImageGrid1D;
typedef std::vector<WorldImageGrid1D> WorldImageGrid2D;

typedef uint32_t DistortionCellCoordinate;
typedef sf::Color PixelColor;

struct WorldImageGrid {

	sf::Vector2f textureQuadrantSize;
	WorldImageGrid2D worldTextureGrid;

	WorldImageGrid(uint32_t gridSizeX, uint32_t gridSizeY, float quadrantSizeX, float quadrantSizeY);
	WorldImage& worldImageFromPixel(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY);

	void pixelSetColor(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY, PixelColor color);
	const PixelColor pixelGetColor(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY);
	const PixelColor pixelGetColorSafe(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY);

private:
	sf::Vector2u fullGridSize;
};


#endif
