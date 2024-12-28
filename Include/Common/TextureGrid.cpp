#include "TextureGrid.hpp"

GridTexture::GridTexture() {
	initialize();
}

bool GridTexture::getDrawActive() const {
	return IsDrawActive;
}
void GridTexture::setDrawActive(bool state) {
	IsDrawActive = state;
}

sf::Vector2u GridTexture::positionGet() {
	return position;
}

TextureGrid::TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<GridTexture>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	TextureGrid2D rows(gridSize.x);

	for (uint32_t x = 0; x < gridSize.x; x++) {

		TextureGrid1D columns(gridSize.y);
		for (uint32_t y = 0; y < gridSize.y; y++) {
			columns[y].create(cellSize.x, cellSize.y);
			columns[y].position = sf::Vector2u(x * cellSizeX, y * cellSizeY);
		}

		rows[x] = std::move(columns);
	}

	cells = std::move(rows);
}


void TextureGrid::texturesAllDeactivate() {
	for (uint16_t i = 0; i < activeTextureIndexes.size(); i++) {
		cellDeactivate(activeTextureIndexes[i]);
	}
}

void TextureGrid::cellActivate(CellCoordinate cellX, CellCoordinate cellY) {
	cellGet(cellX, cellY).setDrawActive(true);
	activeTextureIndexes.push_back(CellVector(cellX, cellY));
}
void TextureGrid::cellActivate(CellVector cellPos) {
	cellActivate(cellPos.x, cellPos.y);
}
void TextureGrid::cellActivateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	cellActivate(coordinatesWorldToCell(worldX, worldY));
}
void TextureGrid::cellActivateFromWorld(WorldVector worldPos) {
	cellActivateFromWorld(worldPos.x, worldPos.y);
}


void TextureGrid::cellDeactivate(CellCoordinate cellX, CellCoordinate cellY) {
	cellGet(cellX, cellY).setDrawActive(false);
}
void TextureGrid::cellDeactivate(CellVector cellPos) {
	cellDeactivate(cellPos.x, cellPos.y);
}
void TextureGrid::cellDeactivateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY) {
	cellDeactivate(coordinatesWorldToCell(worldX, worldY));
}
void TextureGrid::cellDeactivateFromWorld(WorldVector worldPos) {
	cellDeactivateFromWorld(worldPos.x, worldPos.y);
}
