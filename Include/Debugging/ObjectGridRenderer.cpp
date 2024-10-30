#include "ObjectGridRenderer.hpp"

ObjectGridRenderer::ObjectGridRenderer() {
	for (uint16_t i = 0; i < uint16_t(ObjectTypes::SIZE); i++) {
		objectTypeColors.push_back(sf::Color(((i * 1000) % 127) * 2, (((i * 9999) % 255) * 72) % 255, (i * 99999) % 255, 255));
	}
}

void ObjectGridRenderer::objectGridRender(ObjectGrid& objectGrid) {

	if (renderCycle.x == 1 && renderCycle.y == 1) {
		objectGridImage.create(objectGrid.gridGetSizeX(), objectGrid.gridGetSizeY());
		objectGridImage.createMaskFromColor(sf::Color::Black);
	}

	int xStart = (objectGrid.gridGetSizeX() / renderCycleMax) * (renderCycle.x - 1);
	int xEnd = (objectGrid.gridGetSizeX() / renderCycleMax) * renderCycle.x;

	int yStart = (objectGrid.gridGetSizeY() / renderCycleMax) * (renderCycle.y - 1);
	int yEnd = (objectGrid.gridGetSizeY() / renderCycleMax) * renderCycle.y;

	for (uint16_t x = xStart; x < xEnd; x++) {
		for (uint16_t y = yStart; y < yEnd; y++) {

			auto& idsSet = objectGrid.cellTypesGet(x, y);

			if (idsSet.size() <= 0) continue;

			std::vector<ObjectTypes> idsVector(idsSet.begin(), idsSet.end());

			uint16_t r = 0;
			uint16_t g = 0;
			uint16_t b = 0;

			for (uint16_t i = 0; i < idsVector.size(); i++) {
				sf::Color objectTypeColor = objectTypeColors[uint16_t(idsVector[i])];

				r += objectTypeColor.r;
				g += objectTypeColor.g;
				b += objectTypeColor.b;
			}

			uint16_t totalColors = idsVector.size();

			sf::Color pixelColor = sf::Color(r / totalColors, g / totalColors, b / totalColors, 255);

			objectGridImage.setPixel(x, y, pixelColor);

		}
	}

	renderCycleUpdate();
}
const sf::Texture& ObjectGridRenderer::objectGridTextureGet() const {
	return objectGridTexture;
}

void ObjectGridRenderer::renderCycleUpdate() {
	renderCycle.x++;
	if (renderCycle.x > renderCycleMax) {
		renderCycle.x = 1;
		renderCycle.y++;

		if (renderCycle.y > renderCycleMax) {
			renderCycle.y = 1;
			objectGridTexture.loadFromImage(objectGridImage);
		}
	}
}

