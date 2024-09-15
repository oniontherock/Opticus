#ifndef __WORLD_TEXTURE_H__
#define __WORLD_TEXTURE_H__

#include <Auxiliary/ConsoleHandler.hpp>
#include "SFML/Graphics.hpp"

typedef uint32_t PixelCoordinate;
typedef sf::Color PixelColor;

// RenderTexture that holds allows direct reading of pixels.
// Can also have read/write permissions disabled/enabled
struct WorldTexture : sf::RenderTexture {
	bool read = true;
	bool write = true;

	PixelColor getPixel(PixelCoordinate x, PixelCoordinate y);

	void display();

private:
	// boolean for whether the texture was modified since the last update to "image",
	// if it was, "image" will be updated on the next read operation.
	bool modified;

	// image containing the pixel data for our texture
	sf::Image image;

	void imageUpdate();
};

#endif
