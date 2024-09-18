#include "VisionCaster.hpp"

VisionCaster::VisionCaster() {
	sf::Vector2u panelSize = sf::Vector2u(PanelManager::panelGet(PanelName::GameView).viewGet().getSize());
	visionTexture.create(panelSize.x, panelSize.y);

	sf::Vector2u roomSize = GameLevelGrid::levelGet(castPosition.level)->levelSize;
	memoryTexture.create(roomSize.x, roomSize.y);
	memoryTexture.clear(sf::Color(90, 90, 90, 255));

	sf::Texture noiseTexture;
	noiseTexture.loadFromFile("Art/Cobweb Noise.jpg");
	noiseTexture.setRepeated(true);

	sf::Sprite noiseSprite(noiseTexture);
	noiseSprite.setColor(sf::Color(130, 130, 130, 75));
	noiseSprite.setScale(2.f, 2.f);
	noiseSprite.setTextureRect(sf::IntRect(0, 0, roomSize.x, roomSize.y));

	memoryTexture.draw(noiseSprite);
}
VisionCaster::VisionCaster(const VisionCaster& other) {
	visionTexture.create(other.visionTexture.getSize().x, other.visionTexture.getSize().y);

	memoryTexture.create(other.memoryTexture.getSize().x, other.memoryTexture.getSize().y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));

	castPosition = other.castPosition;
}
void VisionCaster::operator= (const VisionCaster& other) {
	visionTexture.create(other.visionTexture.getSize().x, other.visionTexture.getSize().y);

	memoryTexture.create(other.memoryTexture.getSize().x, other.memoryTexture.getSize().y);
	memoryTexture.draw(sf::Sprite(other.memoryTexture.getTexture()));

	castPosition = other.castPosition;
}

const sf::RenderTexture& VisionCaster::visionTextureGet() {
	return visionTexture;
}
const sf::RenderTexture& VisionCaster::renderTextureGet() {
	return memoryTexture;
}

void VisionCaster::update(float fromX, float fromY, float angleTo, float coneSize, uint32_t rayCount) {

	castPosition = sf::Vector2f(fromX, fromY);

	// cast the rays, updating the visionImage
	raysCast(angleTo, coneSize, rayCount);
	// update the memory, this blurs the memory then adds a grayscaled version of the visionImage to it
	memoryUpdate();
}

std::pair<int, int> getClosestFactors(int input) {
	int testNum = (int)sqrt(input);
	while (input % testNum != 0) {
		testNum--;
	}
	return { testNum, input / testNum };
}

void VisionCaster::raysCast(float angleTo, float coneSize, uint32_t rayCount) {

	GameLevel* gameLevel = GameLevelGrid::levelGet(castPosition.level);

	sf::Image visionImage;
	visionImage.create(visionTexture.getSize().x, visionTexture.getSize().y, sf::Color::Transparent);

	// the center of the visionImage
	const sf::Vector2f visionImageCenter = sf::Vector2f(visionTexture.getSize()) / 2.f;
	// center of the GameView panel's view in global room coordinates
	const sf::Vector2f cameraCenterGlobal = PanelManager::panelGet(PanelName::GameView).viewGet().getCenter();
	// center of the GameView panel's view localized around the global cast position,
	const sf::Vector2f cameraCenterLocal = cameraCenterGlobal - castPosition.position;


	// the angular difference (in radians) between two rays.
	const float rayAngleDifference = coneSize / rayCount;

	if (castPosition.position.x < 0 || castPosition.position.x >= gameLevel->levelSize.x || castPosition.position.y < 0 || castPosition.position.y >= gameLevel->levelSize.y) {
		return;
	}

	std::vector<float> multipliers(255);
	for (uint16_t i = 0; i < 255; i++) {
		multipliers[i] = 1.f / (i + 1);
	}

	sf::Uint8 xChunk = 1;
	sf::Uint8 xPoint = 1;
	sf::Uint8 yChunk = 1;
	sf::Uint8 yPoint = 1;

	// the maximum assumed distance a ray can travel
	// note the "assumed", because the ray may have moved more or less, due to distortions.
	// we can use this assumed distance to find where the ray would be on the visionImage if no distortions existed.
	constexpr float maxDist = 525;

	auto* worldImage = &gameLevel->worldGrid.imageGrid.cellGetFromWorld(PixelCoordinate(castPosition.position.x), PixelCoordinate(castPosition.position.y));

	auto& distortionGrid = gameLevel->worldGrid.distortionGrid;

	constexpr double posMultiplier = 1.0 / 255.0;

	for (uint32_t curRayInd = 0; curRayInd < rayCount; curRayInd++) {

		// the rotation (in radians) of the current ray.
		const float rayRotation = angleTo + (float(curRayInd) * rayAngleDifference);
		// the original/unmodified heading of a ray.
		// this is the heading before applying distortions, will be the same to rayHeading in a distortionless environment.
		// used for determining the pixel in the visionImage to write at, because the visionImage should be written to as if there were no distortions.
		const sf::Vector2f rayHeadingOrig = sf::Vector2f(cos(rayRotation), sin(rayRotation));

		sf::Vector2f rayPosition = castPosition.position;
		sf::Vector2f rayHeading = rayHeadingOrig;


		// the assumed dist that the ray has moved.
		// note the "assumed", because the ray may have moved more or less, due to distortions.
		// we can use this assumed distance to find where the ray would be on the visionImage if no distortions existed.
		float curDist = 0.f;
		while (curDist < maxDist) {
			curDist += 1.f;

			auto& distortion = distortionGrid.cellGet(rayPosition.x * distortionGrid.distortionCellMultiplierX, rayPosition.y * distortionGrid.distortionCellMultiplierY);

			// check if there are any distortions at the ray's position
			if (distortion.distortions.size() > 0) {
				// apply the distortion at the rayPosition to the ray.
				distortion.headingApplyDistortion(rayHeading, rayPosition);
				
				if (Vector2fMath::lengthSqrd(rayHeading) <= 0.001f * 0.001f) break;
			}


			// move the rayPosition by the rayHeading.
			// keep in mind that a distortion was just applied to the heading, though the distortion may not have done anything.
			rayPosition += rayHeading;
			// make sure the rayPosition is in the bounds of the level.
			if (rayPosition.x < 0 || rayPosition.x >= gameLevel->levelSize.x || rayPosition.y < 0 || rayPosition.y >= gameLevel->levelSize.y) break;

			// this is a little complicated.
			// remember that the visionImage is the same size as the camera, and when we draw to the visionImage we draw local to the center of the visionImage/camera.
			// but the camera isn't locked directly to the player,
			// that means that if we only draw in the center of the visionImage, and the player isn't directly in the center of the camera,
			// then the rays will be drawn in the wrong position.
			// so we offset the position the rays are drawn at by the amount the camera is offset from the player, and thus they are drawn at the correct position.
			sf::Vector2f visionPixel = (visionImageCenter - cameraCenterLocal) + (rayHeadingOrig * curDist);
			if (visionPixel.x < 0 || visionPixel.x >= visionTexture.getSize().x || visionPixel.y < 0 || visionPixel.y >= visionTexture.getSize().y) break;

			double xDivided = double((rayPosition.x)) * posMultiplier;
			xChunk = static_cast<sf::Uint8>(static_cast<uint8_t>(xDivided));
			xPoint = static_cast<sf::Uint8>(static_cast<uint8_t>((xDivided - xChunk) * 255));

			double yDivided = double((rayPosition.y)) * posMultiplier;
			yChunk = static_cast<sf::Uint8>(static_cast<uint8_t>(yDivided));
			yPoint = static_cast<sf::Uint8>(static_cast<uint8_t>((yDivided - yChunk) * 255));

			visionImage.setPixel(static_cast<uint16_t>(visionPixel.x), static_cast<uint16_t>(visionPixel.y), sf::Color(xChunk, xPoint, yChunk, yPoint));
		}
	}

	sf::Vector2u worldImageTextureSize = worldImage->getTexture().getSize();

	sf::Texture visionImageTexture;
	visionImageTexture.loadFromImage(visionImage);

	sf::Shader shader;
	shader.loadFromFile("Include/Shaders/Raycasting/RayPositionsToWorldColors.glsl", sf::Shader::Fragment);
	shader.setUniform("rayPositions", visionImageTexture);
	shader.setUniform("worldTexture", worldImage->getTexture());
	shader.setUniform("worldSize", sf::Glsl::Vec2(float(worldImageTextureSize.x), float(worldImageTextureSize.y)));

	sf::Sprite visionSprite(visionImageTexture);

	visionTexture.clear(sf::Color::Transparent);
	visionTexture.draw(visionSprite, &shader);
	visionTexture.display();
}
void VisionCaster::memoryUpdate() {
	// blur the memory
	memoryBlur();

	// create a sprite from the visionTexture
	sf::Sprite visionSprite(visionTexture.getTexture());
	// set the visionSprite's position to that of the camera
	visionSprite.setPosition(PanelManager::panelGet(PanelName::GameView).viewRect.getPosition());
	//visionSprite.setColor(sf::Color(255 / 2, 255 / 2, 255 / 2, 255));

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
	float delta = float(TimeHandler::deltaSimulatedGet());

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


