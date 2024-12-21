#include "TextureGrid.hpp"

TextureGrid::TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY) :
	Grid<sf::RenderTexture>::Grid(gridSizeX, gridSizeY, cellSizeX, cellSizeY)
{
	for (uint32_t x = 0; x < gridSize.x; x++) {
		cells.push_back(TextureGrid1D());
		for (uint32_t y = 0; y < gridSize.y; y++) {
			cells[x].push_back(sf::RenderTexture());
			cells[x][y].create(cellSize.x, cellSize.y);
		}
	}
}