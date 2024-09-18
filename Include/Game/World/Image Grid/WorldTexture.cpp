#include "WorldTexture.hpp"


PixelColor WorldTexture::getPixel(PixelCoordinate x, PixelCoordinate y) {

	if (!read) return PixelColor::Black;

	if (modified) {
		imageUpdate();
	}

	return image.getPixel(x, y);
}

void WorldTexture::display() {

	if (!write) {
		ConsoleHandler::consolePrintErr(
		"display called on a WorldTexture that is not marked for writing! \n ensure no draw calls are being made if the WorldTexture is not marked for writing"
		);
		return;
	}

	sf::RenderTexture::display();
	modified = true;
}

void WorldTexture::imageUpdate() {
	//image = getTexture().copyToImage();
	modified = false;
}

