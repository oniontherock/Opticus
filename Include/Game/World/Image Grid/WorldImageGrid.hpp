#ifndef __WORLD_IMAGE_GRID_H__
#define __WORLD_IMAGE_GRID_H__

#include "SFML/Graphics.hpp"
#include "WorldTexture.hpp"

typedef std::vector<WorldTexture> WorldImageGrid1D;
typedef std::vector<WorldImageGrid1D> WorldImageGrid2D;

struct WorldImageGrid {

	// size of the images in the WorldImageGrid
	sf::Vector2f imageSize;
	// size of the WorldImageGrid, so how many WorldImages there are in the X and Y axis
	sf::Vector2u gridSize;
	// size of the WorldImageGrid taking into account the quadrant size, so the actual pixel size of the WorldImageGrid
	sf::Vector2u gridSizeFull;
	WorldImageGrid2D imageGrid;

	WorldImageGrid(uint32_t gridSizeX, uint32_t gridSizeY, float imageSizeX, float imageSizeY);
	WorldTexture& worldImageFromPixel(PixelCoordinate pixelX, PixelCoordinate pixelY);

	//void pixelSetColor(PixelCoordinate pixelX, PixelCoordinate pixelY, PixelColor color);
	const PixelColor pixelGetColor(PixelCoordinate pixelX, PixelCoordinate pixelY);
	const PixelColor pixelGetColorSafe(PixelCoordinate pixelX, PixelCoordinate pixelY);

	void imagesAllReadSet(const bool state);
	void imagesAllWriteSet(const bool state);

};


#endif
