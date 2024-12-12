#include "GrassGenerator.hpp"
#include <Auxiliary/NumberGenerator.hpp>
#include <Auxiliary/VectorMath.hpp>
#include <Auxiliary/Math.hpp>
#include <iostream>

GrassGenerator::GrassGenerator(sf::FloatRect area, uint32_t count) {
	for (uint32_t i = 0; i < count; i++) {
		sf::Vector2f randPoint = sf::Vector2f(RNGf::getRange(area.left, area.left + area.width), RNGf::getRange(area.top, area.top - area.height));

		GrassBlade blade;
		blade.base = sf::Vector3f(randPoint.x, randPoint.y, 0);
		blade.length = RNGf::getRange(16.f, 32.f);

		blade.color = sf::Color(RNGu8::getRange(0, 50), RNGu8::getRange(125, 255), 0, 255);
		
		grassBladeVector.push_back(blade);
	}
}

void GrassGenerator::simulate(float windAngle) {

	windAngle += RNGf::getRange(Mathf::PI / 2);

	sf::Vector2f windDirection = sf::Vector2f(cos(windAngle), sin(windAngle)) * RNGf::getRange(2.5f, 10.f);

	for (GrassBlade& grassCur : grassBladeVector) {
		grassCur.endTarget = grassCur.end + sf::Vector3f(windDirection.x, windDirection.y, 5);

		grassCur.end.z += RNGf::getRange(0.f, 0.5f);

		sf::Vector3f axisBaseTarget = grassCur.endTarget - grassCur.base;
		float axisLen = sqrt((axisBaseTarget.x * axisBaseTarget.x) + (axisBaseTarget.y * axisBaseTarget.y) + (axisBaseTarget.z * axisBaseTarget.z));

		if (axisLen > grassCur.length) {
			axisBaseTarget /= axisLen;
			axisBaseTarget *= grassCur.length;

			grassCur.endTarget = grassCur.base + axisBaseTarget;
		}

		sf::Vector3f axisEndTarget = grassCur.endTarget - grassCur.end;
		grassCur.end = (grassCur.end) + (axisEndTarget * 0.5f);
	}
}
void GrassGenerator::grassSpritesUpdate() {

	grassSprites.clear();

	for (GrassBlade& grassCur : grassBladeVector) {
		sf::Vector3f axisBaseEnd = grassCur.end - grassCur.base;

		float axisAngleLeft = atan2(axisBaseEnd.y, axisBaseEnd.x) - (Mathf::PI * 0.5f);
		float axisAngleRight = atan2(axisBaseEnd.y, axisBaseEnd.x) + (Mathf::PI * 0.5f);

		sf::ConvexShape line;

		line.setPointCount(4);

		line.setPoint(0, sf::Vector2f(cos(axisAngleLeft), sin(axisAngleLeft)));
		line.setPoint(1, sf::Vector2f(cos(axisAngleRight), sin(axisAngleRight)));

		line.setPoint(2, sf::Vector2f(axisBaseEnd.x, axisBaseEnd.y) + sf::Vector2f(cos(axisAngleRight), sin(axisAngleRight)));
		line.setPoint(3, sf::Vector2f(axisBaseEnd.x, axisBaseEnd.y) + sf::Vector2f(cos(axisAngleLeft), sin(axisAngleLeft)));
	
		line.setPosition(sf::Vector2f(grassCur.base.x, grassCur.base.y));
		line.setFillColor(grassCur.color);

		grassSprites.push_back(line);
	}
}
