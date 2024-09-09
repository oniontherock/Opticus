#include "VisionCaster.hpp"

VisionCaster::VisionCaster() {
	sf::Vector2u panelSize = sf::Vector2u(PanelManager::panelGet(PanelName::GameView).viewGet().getSize());
	visionImage.create(panelSize.x, panelSize.y);

	sf::Vector2u roomSize = GameLevelGrid::levelGet(castPosition.level)->levelSize;
	memoryTexture.create(roomSize.x, roomSize.y);
}
VisionCaster::VisionCaster(const VisionCaster& other) {
	visionImage = other.visionImage;

	memoryTexture.create(other.memoryTexture.getSize().x, other.memoryTexture.getSize().y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));

	visionSprite = other.visionSprite;
	memorySprite = other.memorySprite;

	castPosition = other.castPosition;
}
void VisionCaster::operator= (const VisionCaster& other) {
	visionImage = other.visionImage;

	memoryTexture.create(other.memoryTexture.getSize().x, other.memoryTexture.getSize().y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));

	visionSprite = other.visionSprite;
	memorySprite = other.memorySprite;

	castPosition = other.castPosition;
}

void VisionCaster::update(float fromX, float fromY, float angleTo, float coneSize, uint32_t rayCount) {

	castPosition = sf::Vector2f(fromX, fromY);

	spritesUpdate();
	raysCast(angleTo, coneSize, rayCount);
}
const sf::Sprite& VisionCaster::visionSpriteGet() {
	return visionSprite;
}
const sf::Sprite& VisionCaster::memorySpriteGet() {
	return memorySprite;
}
void VisionCaster::spritesUpdate() {

	// get texture from visionImage
	sf::Texture visionTexture;
	visionTexture.loadFromImage(visionImage);
	// create sprite from vision texture.
	visionSprite.setTexture(visionTexture);
	visionSprite.setPosition(castPosition.position - (PanelManager::panelGet(PanelName::GameView).viewGet().getSize() / 2.f));

	memoryTexture.display();
	memorySprite.setTexture(memoryTexture.getTexture());
}

void VisionCaster::raysCast(float angleTo, float coneSize, uint32_t rayCount) {

	GameLevel* gameLevel = GameLevelGrid::levelGet(castPosition.level);

	// the rectangle of the visionImage, used for checking if a position is in vision bounds
	const sf::FloatRect visionImageRect = sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(visionImage.getSize()));
	// the center of the visionImage
	const sf::Vector2f visionImageCenter = visionImageRect.getSize() / 2.f;
	// the rectangle of the level, used for checking if a position is in level bounds
	const sf::FloatRect levelRect = sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(gameLevel->levelSize));

	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / rayCount;

	for (uint32_t curRayInd = 0; curRayInd < rayCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = (angleTo - (coneSize / 2.f)) + (float(curRayInd) * rayAngleDifference);
		// the original/unmodified heading of a ray.
		// this is the heading before applying distortions, will be the same to rayHeading in a distortionless environment.
		// used for determining the pixel in the visionImage to write at, because the visionImage should be written to as if there were no distortions.
		const sf::Vector2f rayHeadingOrig = sf::Vector2f(cos(rayRotation), sin(rayRotation));

		sf::Vector2f rayPosition = castPosition.position;
		sf::Vector2f rayHeading = rayHeadingOrig;

		constexpr float maxDist = 250.f;

		// the assumed dist that the ray has moved.
		// note the "assumed", because the ray may have moved more or less, due to distortions.
		// we can use this assumed distance to find where the ray would be on the visionImage if no distortions existed.
		for (float curDist = 0.f; curDist < maxDist; curDist++) {

			// apply the distortion at the rayPosition to the rayHeading.
			gameLevel->worldGrid.distortionGrid.headingApplyDistortion(rayHeading, sf::Vector2u(rayPosition));
			// move the rayPosition by the rayHeading.
			// keep in mind that a distortion was just applied to the heading, though the distortion may not have done anything.
			rayPosition += rayHeading;
			// make sure the rayPosition is in the bounds of the level.
			if (!levelRect.contains(rayPosition)) break;


			sf::Vector2f visionPixelCoordinate = visionImageCenter + (rayHeadingOrig * curDist);
			if (!visionImageRect.contains(visionPixelCoordinate)) break;

			//sf::Color worldPixelColor = gameLevel->worldGrid.imageGrid.pixelGetColor(PixelCoordinate(rayPosition.x), PixelCoordinate(rayPosition.y));
			
			//std::cout << 1 << std::endl;

			visionImage.setPixel(25, 25, sf::Color(0, 255, 0, 255));
		}
	}
}
void VisionCaster::memoryUpdate() {
	memoryTexture.draw(visionSpriteGet());
}
void VisionCaster::memoryBlur() {

}


