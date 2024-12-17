#ifndef __DISTORTION_CHECK_DRAWER_H__
#define __DISTORTION_CHECK_DRAWER_H__

#include <Graphics.hpp>
#include "../../../ACECS/GameLevel.hpp"

namespace Distortions {
	// checks for distortions when drawing an object, so that sprites don't clip when traveling though portals and such
	// 
	// @param texture: texture to draw on.
	// @param drawableObject: object to draw.
	// @param position: WorldPosition for getting level and ray origin point in that level.
	// @param rayCount: number of rays to launch, increasing this ensures no distortions are missed, but may decrease performance.
	// @param rayMaxDist: maximum distance for a ray to travel when checking for distortions.
	//template <typename T>
	inline void drawCheckDistortions(sf::RenderTexture& texture, sf::Sprite& drawableObject, WorldPosition position, uint16_t rayCount, float rayMaxDist) {

		std::vector<sf::Vector2f> points;

		GameLevel* gameLevel = GameLevelGrid::levelGet(position.level);

		// the angular difference (in radians) between two rays.
		const float rayAngleDifference = Mathf::TAU / rayCount;

		if (position.position.x < 0 || position.position.x >= gameLevel->levelSize.x || position.position.y < 0 || position.position.y >= gameLevel->levelSize.y) {
			return;
		}

		auto& distortionGrid = gameLevel->distortionGrid;

		for (uint32_t curRayInd = 0; curRayInd < rayCount; curRayInd++) {

			// the rotation (in radians) of the current ray.
			const float rayRotation = (float(curRayInd) * rayAngleDifference);
			const sf::Vector2f rayHeadingOrig = sf::Vector2f(cos(rayRotation), sin(rayRotation));

			sf::Vector2f rayPosition = position.position;
			sf::Vector2f rayHeading = sf::Vector2f(cos(rayRotation), sin(rayRotation));


			// the assumed dist that the ray has moved.
			// note the "assumed", because the ray may have moved more or less, due to distortions.
			float curDist = 0.f;
			while (curDist < rayMaxDist) {
				curDist += 1.f;

				sf::Vector2u cellPosition = sf::Vector2u(
					uint32_t(rayPosition.x * distortionGrid.distortionCellMultiplier),
					uint32_t(rayPosition.y * distortionGrid.distortionCellMultiplier)
				);

				auto& distortion = distortionGrid.cellGet(cellPosition);

				// apply the distortion at the rayPosition to the ray.
				distortion.distortionsApplyToRay(rayHeading, rayPosition);

				// move the rayPosition by the rayHeading.
				// keep in mind that a distortion was just applied to the heading, though the distortion may not have done anything.
				rayPosition += rayHeading;
				// make sure the rayPosition is in the bounds of the level.
				if (rayPosition.x < 0 || rayPosition.x >= gameLevel->levelSize.x || rayPosition.y < 0 || rayPosition.y >= gameLevel->levelSize.y) break;
			}


			rayPosition -= rayHeadingOrig * curDist;

			bool found = false;
			// check if the rayPosition already exists in the point vector, if it does, don't add this one to it
			for (uint32_t i = 0; i < points.size(); i++) {
				if (Mathf::approxEquals(rayPosition.x, points[i].x, 0.1f) && Mathf::approxEquals(rayPosition.y, points[i].y, 0.1f)) {
					found = true;
					break;
				}
			}
			if (!found) points.push_back(rayPosition);
		}
		for (uint32_t i = 0; i < points.size(); i++) {
			drawableObject.setPosition(points[i]);
			texture.draw(drawableObject);
		}
	}
}

#endif
