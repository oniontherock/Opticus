#include "WorldImageGrid.hpp"

WorldImageGrid::WorldImageGrid(uint32_t gridSizeX, uint32_t gridSizeY, float quadrantSizeX, float quadrantSizeY) {

	fullGridSize = sf::Vector2u(gridSizeX * uint32_t(quadrantSizeX), gridSizeY * uint32_t(quadrantSizeY));

	textureQuadrantSize = sf::Vector2f(quadrantSizeX, quadrantSizeY);

	WorldImageGrid2D rows(gridSizeX);

	for (uint32_t x = 0; x < gridSizeX; x++) {

		WorldImageGrid1D columns(gridSizeY);
		for (uint32_t y = 0; y < gridSizeY; y++) {
			columns[y].create(uint32_t(textureQuadrantSize.x), uint32_t(textureQuadrantSize.y));
		}

		rows[x] = std::move(columns);
	}

	worldTextureGrid = std::move(rows);
}

WorldImage& WorldImageGrid::worldImageFromPixel(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY) {
	return worldTextureGrid[static_cast<uint32_t>(pixelX / textureQuadrantSize.x)][static_cast<uint32_t>(pixelY / textureQuadrantSize.y)];
}

void WorldImageGrid::pixelSetColor(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY, PixelColor color) {
	worldImageFromPixel(pixelX, pixelY).setPixel(pixelX, pixelY, color);
}

const PixelColor WorldImageGrid::pixelGetColor(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY) {
	return worldImageFromPixel(pixelX, pixelY).getPixel(pixelX, pixelY);
}

const PixelColor WorldImageGrid::pixelGetColorSafe(DistortionCellCoordinate pixelX, DistortionCellCoordinate pixelY) {
	if (pixelX < 0 || pixelX >= fullGridSize.x || pixelY < 0 || pixelY >= fullGridSize.y) return sf::Color::Black;

	return pixelGetColor(pixelX, pixelY);
}

