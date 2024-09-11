#include "VisionCaster.hpp"

VisionCaster::VisionCaster() {
	sf::Vector2u panelSize = sf::Vector2u(PanelManager::panelGet(PanelName::GameView).viewGet().getSize());
	visionImage.create(panelSize.x, panelSize.y);

	sf::Vector2u roomSize = GameLevelGrid::levelGet(castPosition.level)->levelSize;
	memoryTexture.create(roomSize.x, roomSize.y);
}
VisionCaster::VisionCaster(const VisionCaster& other) {
	visionImage.create(other.visionImage.getSize().x, other.visionImage.getSize().y);

	memoryTexture.create(other.memoryTexture.getSize().x, other.memoryTexture.getSize().y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));

	castPosition = other.castPosition;
}
void VisionCaster::operator= (const VisionCaster& other) {
	visionImage.create(other.visionImage.getSize().x, other.visionImage.getSize().y);

	memoryTexture.create(other.memoryTexture.getSize().x, other.memoryTexture.getSize().y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));

	castPosition = other.castPosition;
}

const sf::Image& VisionCaster::visionImageGet() {
	return visionImage;
}
const sf::RenderTexture& VisionCaster::renderTextureGet() {
	return memoryTexture;
}

void VisionCaster::update(float fromX, float fromY, float angleTo, float coneSize, uint32_t rayCount) {

	// clear the visionImage by creating a new image and masking out the color black (which is the default color a new image has), thus creating a totally transparent image
	sf::Vector2u panelSize = sf::Vector2u(PanelManager::panelGet(PanelName::GameView).viewGet().getSize());
	visionImage.create(panelSize.x, panelSize.y);
	visionImage.createMaskFromColor(sf::Color::Black);

	castPosition = sf::Vector2f(fromX, fromY);

	// cast the rays, updating the visionImage
	raysCast(angleTo, coneSize, rayCount);
	// update the memory, this blurs the memory then adds a grayscaled version of the visionImage to it
	memoryUpdate();
}

void VisionCaster::raysCast(float angleTo, float coneSize, uint32_t rayCount) {

	GameLevel* gameLevel = GameLevelGrid::levelGet(castPosition.level);

	// the center of the visionImage
	const sf::Vector2f visionImageCenter = sf::Vector2f(visionImage.getSize()) / 2.f;
	// center of the GameView panel's view in global room coordinates
	const sf::Vector2f cameraCenterGlobal = PanelManager::panelGet(PanelName::GameView).viewGet().getCenter();
	// center of the GameView panel's view localized around the global cast position,
	const sf::Vector2f cameraCenterLocal = cameraCenterGlobal - castPosition.position;

	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / rayCount;

	auto& worldImage = gameLevel->worldGrid.imageGrid.worldImageFromPixel(0, 0);
	

	for (uint32_t curRayInd = 0; curRayInd < rayCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = angleTo + (float(curRayInd) * rayAngleDifference);
		// the original/unmodified heading of a ray.
		// this is the heading before applying distortions, will be the same to rayHeading in a distortionless environment.
		// used for determining the pixel in the visionImage to write at, because the visionImage should be written to as if there were no distortions.
		const sf::Vector2f rayHeadingOrig = sf::Vector2f(cos(rayRotation), sin(rayRotation));

		sf::Vector2f rayPosition = castPosition.position;
		sf::Vector2f rayHeading = rayHeadingOrig;

		// the maximum assumed distance a ray can travel
		// note the "assumed", because the ray may have moved more or less, due to distortions.
		// we can use this assumed distance to find where the ray would be on the visionImage if no distortions existed.
		constexpr float maxDist = 525;

		// the assumed dist that the ray has moved.
		// note the "assumed", because the ray may have moved more or less, due to distortions.
		// we can use this assumed distance to find where the ray would be on the visionImage if no distortions existed.
		for (float curDist = 0.f; curDist < maxDist; curDist++) {

			// apply the distortion at the rayPosition to the rayHeading.
			gameLevel->worldGrid.distortionGrid.headingApplyDistortion(rayHeading, DistortionCellCoordinate(rayPosition.x), DistortionCellCoordinate(rayPosition.y));
			// move the rayPosition by the rayHeading.
			// keep in mind that a distortion was just applied to the heading, though the distortion may not have done anything.
			rayPosition += rayHeading;
			// make sure the rayPosition is in the bounds of the level.
			if (rayPosition.x < 0 || rayPosition.x >= gameLevel->levelSize.x || rayPosition.y < 0 || rayPosition.y >= gameLevel->levelSize.y) break;

			sf::Color worldPixelColor = worldImage.getPixel(uint32_t(rayPosition.x), uint32_t(rayPosition.y));

			// this is a little complicated.
			// remember that the visionImage is the same size as the camera, and when we draw to the visionImage we draw local to the center of the visionImage/camera.
			// but the camera isn't locked directly to the player,
			// that means that if we only draw in the center of the visionImage, and the player isn't directly in the center of the camera,
			// then the rays will be drawn in the wrong position.
			// so we offset the position the rays are drawn at by the amount the camera is offset from the player, and thus they are drawn at the correct position.
			sf::Vector2f visionPixel = (visionImageCenter - cameraCenterLocal) + (rayHeadingOrig * curDist);
			if (visionPixel.x < 0 || visionPixel.x >= visionImage.getSize().x || visionPixel.y < 0 || visionPixel.y >= visionImage.getSize().y) break;
			
			visionImage.setPixel(uint32_t(visionPixel.x), uint32_t(visionPixel.y), worldPixelColor);
		}
	}
}
void VisionCaster::memoryUpdate() {
	// blur the memory
	memoryBlur();

	// create a texture from the visionImage
	sf::Texture visionTexture;
	visionTexture.loadFromImage(visionImage);
	// create a sprite from the visionTexture
	sf::Sprite visionSprite(visionTexture);
	// set the visionSprite's position to that of the camera
	visionSprite.setPosition(PanelManager::panelGet(PanelName::GameView).viewRect.getPosition());

	// load the grayscale shader
	sf::Shader grayscaleShader;
	grayscaleShader.loadFromFile("Include/Shaders/Grayscale.glsl", sf::Shader::Fragment);

	// draw the visionSprite to the memoryTexture with a grayscale applied
	memoryTexture.draw(visionSprite, &grayscaleShader);
	// display the memoryTexture
	memoryTexture.display();
}
void VisionCaster::memoryBlur() {

	// get simulated delta
	double delta = TimeHandler::deltaSimulatedGet();

	// update the blurCooldown, if it is ready, blur the memoryTexture a bit
	if (blurCooldown.updateAutoReset(delta)) {
		// create a sprite from the memoryTexture
		sf::Sprite memorySprite(memoryTexture.getTexture());

		// load the blur shader
		sf::Shader blurShader;
		blurShader.loadFromFile("Include/Shaders/Blur.glsl", sf::Shader::Fragment);
		blurShader.setUniform("directions", 4.f);
		blurShader.setUniform("quality", 8.f);
		blurShader.setUniform("size", 1.f);

		// draw to the memoryTexture with the memorySprite with the blur shader applied, effectively bluring the memory.
		memoryTexture.draw(memorySprite, &blurShader);
	}
}


