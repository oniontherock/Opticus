#include "WorldImageGrid.hpp"

WorldImageGrid::WorldImageGrid(uint32_t gridSizeX, uint32_t gridSizeY, float imageSizeX, float imageSizeY) {

	gridSize = sf::Vector2u(gridSizeX, gridSizeY);

	gridSizeFull = sf::Vector2u(gridSize.x * uint32_t(imageSizeX), gridSize.y * uint32_t(imageSizeY));

	imageSize = sf::Vector2f(imageSizeX, imageSizeY);


	WorldImageGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		WorldImageGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y].create(uint32_t(imageSize.x), uint32_t(imageSize.y));
		}

		rows[x] = std::move(columns);
	}

	imageGrid = std::move(rows);
}

WorldTexture& WorldImageGrid::worldImageFromPixel(PixelCoordinate pixelX, PixelCoordinate pixelY) {
	return imageGrid[static_cast<uint32_t>(pixelX / imageSize.x)][static_cast<uint32_t>(pixelY / imageSize.y)];
}

//void WorldImageGrid::pixelSetColor(PixelCoordinate pixelX, PixelCoordinate pixelY, PixelColor color) {
//	//worldImageFromPixel(pixelX, pixelY).setPixel(pixelX, pixelY, color);
//}

const PixelColor WorldImageGrid::pixelGetColor(PixelCoordinate pixelX, PixelCoordinate pixelY) {
	return worldImageFromPixel(pixelX, pixelY).getTexture().copyToImage().getPixel(pixelX, pixelY);
}

const PixelColor WorldImageGrid::pixelGetColorSafe(PixelCoordinate pixelX, PixelCoordinate pixelY) {
	if (pixelX < 0 || pixelX >= gridSizeFull.x || pixelY < 0 || pixelY >= gridSizeFull.y) return sf::Color::Black;

	return pixelGetColor(pixelX, pixelY);
}


void WorldImageGrid::imagesAllReadSet(const bool state) {
	for (uint16_t x = 0; x < imageGrid.size(); x++) {
		for (uint16_t y = 0; y < imageGrid[x].size(); y++) {
			imageGrid[x][y].read = state;
		}
	}
}
void WorldImageGrid::imagesAllWriteSet(const bool state) {
	for (uint16_t x = 0; x < imageGrid.size(); x++) {
		for (uint16_t y = 0; y < imageGrid[x].size(); y++) {
			imageGrid[x][y].write = state;
		}
	}
}
