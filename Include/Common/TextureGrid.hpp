#ifndef __TEXTURE_GRID_H__
#define __TEXTURE_GRID_H__

#include "Grid.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// render texture that has a boolean representing whether or not it's active
struct GridTexture : sf::RenderTexture {
	GridTexture();

	friend class TextureGrid;

	bool getDrawActive() const;

	sf::Vector2u positionGet();

private:

	void setDrawActive(bool state);

	bool IsDrawActive = false;
	
	sf::Vector2u position;
};

typedef std::vector<GridTexture> TextureGrid1D;
typedef std::vector<TextureGrid1D> TextureGrid2D;

struct TextureGrid : Grid<GridTexture> {
	TextureGrid(uint32_t gridSizeX, uint32_t gridSizeY, float cellSizeX, float cellSizeY);

	void texturesAllDeactivate();

	void cellActivate(CellCoordinate cellX, CellCoordinate cellY);
	void cellActivate(CellVector cellPos);
	void cellActivateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	void cellActivateFromWorld(WorldVector worldPos);

	void cellDeactivate(CellCoordinate cellX, CellCoordinate cellY);
	void cellDeactivate(CellVector cellPos);
	void cellDeactivateFromWorld(WorldCoordinate worldX, WorldCoordinate worldY);
	void cellDeactivateFromWorld(WorldVector worldPos);

	std::vector<CellVector> activeTextureIndexes;
};

#endif
