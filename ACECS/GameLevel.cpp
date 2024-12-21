#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include <Auxiliary/NumberGenerator.hpp>

GameLevel::GameLevel() :
	levelSize(sf::Vector2u(4096, 4096)),
	distortionGrid(DistortionGrid(levelSize.x, levelSize.y)),
	objectGrid(levelSize.x / 4, levelSize.y / 4, 4.f, 4.f),
	roomGrid(RoomGrid(8, 8, 32, 32, 16.f, 16.f)),
	levelGenerator(LevelGenerator()),
	aStarGrid(AStarGrid(levelSize.x / 16, levelSize.y / 16, 16.f, 16.f)),
	pathGenerator(PathGenerator())
{
	worldTextureStatic.create(levelSize.x, levelSize.y);
	worldTextureDynamic.create(levelSize.x, levelSize.y);

	entities = std::vector<EntityId>();
}

GameLevel::GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ) :
	GameLevel()
{
	levelPosition = LevelPosition(_idX, _idY, _idZ);
}

GameLevel::GameLevel(LevelPosition _id) :
	GameLevel(_id.x, _id.y, _id.z)
{}

void GameLevel::grassDraw() {
	backgroundTexture.create(levelSize.x, levelSize.y);

	sf::VertexArray lines;
	lines.setPrimitiveType(sf::Lines);

	uint32_t grassBladeCount = 5000000;

	for (uint32_t i = 0; i < grassBladeCount; i++) {

		sf::Color color = sf::Color(RNGf::getRange(50, 125), RNGf::getRange(100, 255), RNGf::getRange(0, 25), 85);

		sf::Vertex lineStart;
		lineStart.color = color;
		lineStart.position = sf::Vector2f(RNGf::getRange(0.f, levelSize.x), RNGf::getRange(0, levelSize.y));

		sf::Vertex lineEnd;
		lineEnd.color = color;

		// create vector with random heading
		sf::Vector2f lineEndOffset = sf::Vector2f(cos(RNGf::getFullRange(Mathf::PI)), sin(RNGf::getFullRange(Mathf::PI)));
		// scale length of vector
		lineEndOffset = Vector2fMath::lengthSet(lineEndOffset, RNGf::getRange(3.f, 12.f));

		lineEnd.position = lineStart.position + lineEndOffset;

		lines.append(lineStart);
		lines.append(lineEnd);


		sf::Vector2f axis = lineEnd.position - lineStart.position;
		float angleSide = atan2(axis.y, axis.x) + (Mathf::PI * 0.5f);

		sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(1.f, 2.f));

		sf::Vertex lineStartOffsetted = lineStart;
		lineStartOffsetted.position += offset;

		sf::Vertex lineEndOffsetted = lineEnd;
		lineStartOffsetted.position += offset;

		lines.append(lineStartOffsetted);
		lines.append(lineEndOffsetted);
	}
	uint32_t leafCount = 50000;

	for (uint32_t i = 0; i < leafCount; i++) {

		sf::Color color = sf::Color(RNGf::getRange(75, 125), RNGf::getRange(25, 75), RNGf::getRange(0, 25), 85);

		sf::Vertex lineStart;
		lineStart.color = color;
		lineStart.position = sf::Vector2f(RNGf::getRange(0.f, levelSize.x), RNGf::getRange(0, levelSize.y));

		sf::Vertex lineEnd;
		lineEnd.color = color;

		// create vector with random heading
		sf::Vector2f lineEndOffset = sf::Vector2f(cos(RNGf::getFullRange(Mathf::PI)), sin(RNGf::getFullRange(Mathf::PI)));
		// scale length of vector
		lineEndOffset = Vector2fMath::lengthSet(lineEndOffset, RNGf::getRange(3.f, 12.f));

		lineEnd.position = lineStart.position + lineEndOffset;

		lines.append(lineStart);
		lines.append(lineEnd);


		sf::Vector2f axis = lineEnd.position - lineStart.position;
		float angleSide = atan2(axis.y, axis.x) + (Mathf::PI * 0.5f);

		sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(1.f, 2.f));

		sf::Vertex lineStartOffsetted = lineStart;
		lineStartOffsetted.position += offset;

		sf::Vertex lineEndOffsetted = lineEnd;
		lineStartOffsetted.position += offset;

		lines.append(lineStartOffsetted);
		lines.append(lineEndOffsetted);
	}


	backgroundTexture.draw(lines);
	backgroundTexture.display();

}
void GameLevel::pathsGenerate() {
	pathGenerator.pathGenerate(sf::Vector2f(256, levelSize.y / 2), sf::Vector2f(levelSize.x - 256, levelSize.y / 2));
}
void GameLevel::pathsDraw() {
	pathsTexture.create(levelSize.x, levelSize.y);

	sf::VertexArray lines;
	lines.setPrimitiveType(sf::Lines);

	const auto& path = pathGenerator.pathGet();
	const auto& connections = pathGenerator.connectionsGet();

	constexpr float circleSize = 256.f;

	std::vector<sf::Vector2f> points;
	for (uint16_t i = 0; i < connections.size(); i++) {

		PointIndex from = connections[i].first;
		PointIndex to = connections[i].second;

		sf::Vector2f pointFrom = path[from].position;
		sf::Vector2f pointTo = path[to].position;

		constexpr float desiredDist = 16.f;

		points.push_back(pointFrom);
		
		float dist = Vector2fMath::distSqrd(pointFrom, pointTo);
		while (dist > desiredDist * desiredDist) {

			dist = Vector2fMath::distSqrd(pointFrom, pointTo);

			if (dist < (desiredDist * 2.f) * (desiredDist * 2.f)) {
				pointFrom = pointTo;
			}
			else {
				pointFrom += Vector2fMath::lengthSet(Vector2fMath::axis(pointFrom, pointTo), desiredDist);
				pointFrom += sf::Vector2f(RNGf::getRange(32.f), RNGf::getRange(32.f));
			}
			points.push_back(pointFrom);
		}
	}

	const uint32_t lineCount = 500 * points.size();

	for (uint32_t i = 0; i < lineCount; i++) {

		sf::Vector2f pointPosition = points[RNGu16::getUnder(points.size())];

		sf::Vector2f averagedOffset;
		uint16_t weight = 16;
		for (uint16_t weightIteration = 0; weightIteration < weight; weightIteration++) {
			averagedOffset += sf::Vector2f(RNGf::getFullRange(circleSize), RNGf::getFullRange(circleSize));
		}
		averagedOffset /= float(weight);

		sf::Vector2f linePosition = pointPosition + Vector2fMath::lengthLimit(averagedOffset, circleSize);

		float distToNearestPointSqrd = 9999999999999999;
		for (uint32_t i = 0; i < points.size(); i++) {

			float distSqrd = Vector2fMath::distSqrd(points[i], linePosition);
			if (distSqrd < distToNearestPointSqrd) {
				distToNearestPointSqrd = distSqrd;
			}
		}

		float offsetDistFromCenter = 1.f - (sqrt(distToNearestPointSqrd) / circleSize);
		offsetDistFromCenter = offsetDistFromCenter * offsetDistFromCenter;

		sf::Color color = sf::Color(RNGf::getRange(25, 125) * offsetDistFromCenter, RNGf::getRange(25, 75) * offsetDistFromCenter, RNGf::getRange(0, 0) * offsetDistFromCenter, 85);

		sf::Vertex lineStart;
		lineStart.color = color;
		lineStart.position = linePosition;

		sf::Vertex lineEnd;
		lineEnd.color = color;

		// create vector with random heading
		sf::Vector2f lineEndOffset = sf::Vector2f(cos(RNGf::getFullRange(Mathf::PI)), sin(RNGf::getFullRange(Mathf::PI)));
		// scale length of vector
		lineEndOffset = Vector2fMath::lengthSet(lineEndOffset, RNGf::getRange(3.f, 12.f));

		lineEnd.position = lineStart.position + lineEndOffset;

		lines.append(lineStart);
		lines.append(lineEnd);


		sf::Vector2f axis = lineEnd.position - lineStart.position;
		float angleSide = atan2(axis.y, axis.x) + (Mathf::PI * 0.5f);

		sf::Vector2f offset = Vector2fMath::lengthSet(sf::Vector2f(cos(angleSide), sin(angleSide)), RNGf::getRange(1.f, 2.f));

		sf::Vertex lineStartOffsetted = lineStart;
		lineStartOffsetted.position += offset;

		sf::Vertex lineEndOffsetted = lineEnd;
		lineStartOffsetted.position += offset;

		lines.append(lineStartOffsetted);
		lines.append(lineEndOffsetted);
	}

	pathsTexture.draw(lines);
	pathsTexture.display();
}
