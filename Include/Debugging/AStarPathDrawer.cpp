#include "AStarPathDrawer.hpp"

sf::RenderTexture AStarPathDrawer::pathsTexture;

void AStarPathDrawer::pathsTextureReset() {
	pathsTexture.create(4096, 4096);
	pathsTexture.clear(sf::Color::Transparent);
}

void AStarPathDrawer::pathDraw(AStarPath path) {

	sf::CircleShape circleShape(16);
	circleShape.setFillColor(sf::Color::Blue);

	for (uint16_t i = 0; i < path.size(); i++) {
		circleShape.setPosition(path[i]);
		pathsTexture.draw(circleShape);
	}
}

