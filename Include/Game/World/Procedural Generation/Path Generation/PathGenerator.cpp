#include "PathGenerator.hpp"
#include <Auxiliary/VectorMath.hpp>
#include <Auxiliary/Math.hpp>
#include <iostream>
#include <cmath>
#include <Auxiliary/NumberGenerator.hpp>

PathPoint::PathPoint(sf::Vector2f _position, PointIndex _index) {
	position = _position;
	index = _index;
}

PointIndex PathGenerator::pointGetFromPosition(sf::Vector2f position) {
	for (PointIndex i = 0; i < points.size(); i++) {
		if (Vector2fMath::distSqrd(points[i].position, position) < 16.f * 16.f) {
			return i;
		}
	}
}
PointIndex PathGenerator::pointGetClosest(sf::Vector2f position) {

	PointIndex lowestInd = 0;
	float lowestDistSqrd = 9999999999999999999;

	for (PointIndex i = 0; i < points.size(); i++) {
		
		float distSqrd = Vector2fMath::distSqrd(points[i].position, position);

		if (distSqrd < lowestDistSqrd) {
			lowestInd = i;
			lowestDistSqrd = distSqrd;
		}
	}

	return lowestInd;
}

void PathGenerator::pointsConnect(PointIndex indA, PointIndex indB) {
	connections.push_back(PointConnection(indA, indB));
}
void PathGenerator::pointsConnect(const PathPoint& a, const PathPoint& b) {
	pointsConnect(a.index, b.index);
}

PathPoint& PathGenerator::pointCreate(sf::Vector2f position) {
	PathPoint point = PathPoint(position, points.size());
	points.push_back(point);
	
	return points[points.size() - 1];
}

void PathGenerator::pointGenerate(PathPoint& point, const PathPoint& pointEnd, uint16_t generation, const uint16_t generationMax) {

	sf::Vector2f axisToEnd = Vector2fMath::axis(point.position, pointEnd.position);

	if (generation < generationMax) {

		uint16_t childrenMin = 1;
		uint16_t childrenMax = Mathu16::clamp(std::min(3, generationMax - generation), childrenMin, 99999);
		
		uint16_t childrenCount;
		if (childrenMin == childrenMax) {
			childrenCount = 1;
		}
		else {
			childrenCount = RNGu16::getRange(childrenMin, childrenMax);
		}

		for (uint16_t i = 0; i < childrenCount; i++) {
			sf::Vector2f childPosition = point.position + axisToEnd / float(generationMax - generation);
			childPosition += sf::Vector2f(RNGf::getRange(128.f), RNGf::getFullRange(1024));

			PathPoint& pointChild = pointCreate(childPosition);

			pointsConnect(point, pointChild);

			pointGenerate(pointChild, pointEnd, generation + 1, generationMax);
		}
	}
	else {
		pointsConnect(point, pointEnd);
	}
}
void PathGenerator::pathGenerate(sf::Vector2f pointStartPosition, sf::Vector2f pointEndPosition) {
	
	PathPoint& pointStart = pointCreate(pointStartPosition);
	PathPoint& pointEnd = pointCreate(pointEndPosition);

	pointGenerate(pointStart, pointEnd, 0, 5);
}

const std::vector<PathPoint>& PathGenerator::pathGet() {
	return points;
}
const std::vector<PointConnection>& PathGenerator::connectionsGet() {
	return connections;
}
