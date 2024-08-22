#ifndef __WORLD_TEXTURE_HOLDER_H__
#define __WORLD_TEXTURE_HOLDER_H__

#include "SFML/Graphics.hpp"

typedef sf::Image WorldImage;
typedef std::vector<WorldImage> WorldImageGrid1D;
typedef std::vector<WorldImageGrid1D> WorldImageGrid2D;

typedef uint32_t DistortionCellCoordinate;
typedef sf::Color PixelColor;

struct WorldImageGrid {

	static sf::Vector2f textureQuadrantSize;
	static WorldImageGrid2D worldTextureGrid;

	static void worldTextureGridInitialize(uint32_t gridSizeX, uint32_t gridSizeY, float quadrantSizeX, float quadrantSizeY);
	static WorldImage& worldImageFromPixel(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY);

	static void pixelSetColor(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY, PixelColor color);
	static const PixelColor pixelGetColor(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY);
	static const PixelColor pixelGetColorSafe(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY);

private:
	static sf::Vector2u fullGridSize;
};


#endif
