#include "VisionCaster.hpp"

VisionCaster::VisionCaster() {
	sf::Vector2u panelSize = sf::Vector2u(PanelManager::panelGet(PanelName::GameView).viewGet().getSize());
	visionTexture.create(panelSize.x, panelSize.y);

	textureToSee = nullptr;
}
VisionCaster::VisionCaster(sf::Vector2f _castPosition) :
	VisionCaster()
{
	castPosition = _castPosition;
}


VisionCaster::VisionCaster(const VisionCaster& other) {
	visionTexture.create(other.visionTexture.getSize().x, other.visionTexture.getSize().y);

	castPosition = other.castPosition;

	textureToSee = other.textureToSee;
}
void VisionCaster::operator= (const VisionCaster& other) {
	visionTexture.create(other.visionTexture.getSize().x, other.visionTexture.getSize().y);

	castPosition = other.castPosition;
}

void VisionCaster::textureToSeeSet(sf::RenderTexture& texture) {
	textureToSee = &texture;
}

void VisionCaster::visionClear() {
	visionImage.create(visionTexture.getSize().x, visionTexture.getSize().y, sf::Color::Transparent);
	visionImageCenter = sf::Vector2f(visionTexture.getSize()) / 2.f;

	visionTexture.clear(sf::Color::Transparent);
}

void VisionCaster::visionDisplay() {
	sf::Vector2u worldImageTextureSize = textureToSee->getTexture().getSize();

	sf::Texture visionImageTexture;
	visionImageTexture.loadFromImage(visionImage);

	sf::Shader shader;
	shader.loadFromFile("Include/Shaders/Raycasting/RayPositionsToWorldColors.glsl", sf::Shader::Fragment);
	shader.setUniform("rayPositions", visionImageTexture);
	shader.setUniform("worldTexture", textureToSee->getTexture());
	shader.setUniform("worldSize", sf::Glsl::Vec2(float(worldImageTextureSize.x), float(worldImageTextureSize.y)));
	shader.setUniform("raysOrigin", sf::Glsl::Vec2(sf::Vector2f(visionImageCenter - cameraCenterLocal)));

	sf::Sprite visionSprite(visionImageTexture);

	visionTexture.draw(visionSprite, &shader);

	visionTexture.display();
}

const sf::RenderTexture& VisionCaster::visionTextureGet() {
	return visionTexture;
}
void VisionCaster::update(float fromX, float fromY, float angleTo, float coneSize, float rayMaxDist, uint32_t rayCount) {

	castPosition.position = sf::Vector2f(fromX, fromY);
	castAngle = angleTo - (coneSize / 2.f);

	cameraCenterGlobal = PanelManager::panelGet(PanelName::GameView).viewGet().getCenter();
	cameraCenterLocal = cameraCenterGlobal - castPosition.position;

	// cast the rays, updating the visionImage
	raysCast(coneSize, rayMaxDist, rayCount);
}

static void raysCastAndUpdateVisionImage(uint32_t rayCount, float angleTo, float rayAngleDifference, sf::Vector2f castPosition, float rayMaxDist, DistortionGrid& distortionGrid, sf::Image& visionImage, sf::Vector2u gameLevelSize, sf::Vector2f cameraCenter, sf::Vector2u visionTextureSize) {

	constexpr double posMultiplier = 1.0 / 255.0;

	for (uint32_t curRayInd = 0; curRayInd < rayCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = angleTo + (float(curRayInd) * rayAngleDifference);
		// the original/unmodified heading of a ray.
		// this is the heading before applying distortions, will be the same to rayHeading in a distortionless environment.
		// used for determining the pixel in the visionImage to write at, because the visionImage should be written to as if there were no distortions.
		const sf::Vector2f rayHeadingOrig = sf::Vector2f(cos(rayRotation), sin(rayRotation));


		sf::Vector2f rayPosition = castPosition;
		sf::Vector2f rayHeading = rayHeadingOrig;

		sf::Vector2u cellPositionPrev;

		// the assumed dist that the ray has moved.
		// note the "assumed", because the ray may have moved more or less, due to distortions.
		// we can use this assumed distance to find where the ray would be on the visionImage if no distortions existed.
		float curDist = 0.f;
		while (curDist < rayMaxDist) {
			curDist += 1.f;

			sf::Vector2u cellPosition = sf::Vector2u(
				uint32_t(rayPosition.x * distortionGrid.distortionCellMultiplier),
				uint32_t(rayPosition.y * distortionGrid.distortionCellMultiplier)
			);

			auto& distortion = distortionGrid.cellGet(cellPosition);

			cellPositionPrev = cellPosition;

			// apply the distortion at the rayPosition to the ray.
			distortion.distortionApplyToRay(rayHeading, rayPosition);

			// move the rayPosition by the rayHeading.
			// keep in mind that a distortion was just applied to the heading, though the distortion may not have done anything.
			rayPosition += rayHeading;
			// make sure the rayPosition is in the bounds of the level.
			if (rayPosition.x < 0 || rayPosition.x >= gameLevelSize.x || rayPosition.y < 0 || rayPosition.y >= gameLevelSize.y) break;

			// this is a little complicated.
			// remember that the visionImage is the same size as the camera, and when we draw to the visionImage we draw local to the center of the visionImage/camera.
			// but the camera isn't locked directly to the player,
			// that means that if we only draw in the center of the visionImage, and the player isn't directly in the center of the camera,
			// then the rays will be drawn in the wrong position.
			// so we offset the position the rays are drawn at by the amount the camera is offset from the player, and thus they are drawn at the correct position.
			sf::Vector2f visionPixel = (cameraCenter) + (rayHeadingOrig * curDist);
			if (visionPixel.x < 0 || visionPixel.x >= visionTextureSize.x || visionPixel.y < 0 || visionPixel.y >= visionTextureSize.y) break;

			double xDivided = double(rayPosition.x) * posMultiplier;
			sf::Uint8 xChunk = static_cast<sf::Uint8>(static_cast<uint8_t>(xDivided));
			sf::Uint8 xPoint = static_cast<sf::Uint8>(static_cast<uint8_t>((xDivided - xChunk) * 255));

			double yDivided = double(rayPosition.y) * posMultiplier;
			sf::Uint8 yChunk = static_cast<sf::Uint8>(static_cast<uint8_t>(yDivided));
			sf::Uint8 yPoint = static_cast<sf::Uint8>(static_cast<uint8_t>((yDivided - yChunk) * 255));

			visionImage.setPixel(static_cast<uint16_t>(visionPixel.x), static_cast<uint16_t>(visionPixel.y), sf::Color(xChunk, xPoint, yChunk, yPoint));
		}
	}
}
void VisionCaster::raysCast(float coneSize, float rayMaxDist, uint32_t rayCount) {

	GameLevel* gameLevel = GameLevelGrid::levelGet(castPosition.level);

	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / rayCount;

	if (castPosition.position.x < 0 || castPosition.position.x >= gameLevel->levelSize.x || castPosition.position.y < 0 || castPosition.position.y >= gameLevel->levelSize.y) {
		return;
	}

	auto& distortionGrid = gameLevel->distortionGrid;

	raysCastAndUpdateVisionImage(rayCount, castAngle, rayAngleDifference, castPosition.position, rayMaxDist, distortionGrid, visionImage, gameLevel->levelSize, visionImageCenter - cameraCenterLocal, visionTexture.getSize());
}

