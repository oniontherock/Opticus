#ifndef __GAME_LEVEL_H__
#define __GAME_LEVEL_H__

#include "../Include/Game/Pathfinding/AStar/Grid/AStarGrid.hpp"
#include "../Include/Game/World/Distortions/DistortionGrid.hpp"
#include "../Include/Game/World/Level Generator/LevelGenerator.hpp"
#include "../Include/Game/World/Objects/Object Grid/ObjectGrid.hpp"
#include "../Include/Game/World/Room Grid/RoomGrid.hpp"
#include "SFML/Graphics.hpp"
#include "World/Level.hpp"
#include "World/LevelGrid.hpp"

// example of the game level class,
// 
// note that there should generally only be one type of GameLevel,
// the reason GameLevel inherits BaseLevel and needs to be down-casted is so the user can define their own variables inside of GameLevel.
// the GameLevel and BaseLevel relationship isn't like the Component relationship where there can be many types of components,
struct GameLevel : public BaseLevel {

	GameLevel();
	GameLevel(LevelCoordinate _idX, LevelCoordinate _idY, LevelCoordinate _idZ);
	GameLevel(LevelPosition _id);

	sf::Vector2u levelSize;

	// set of EntityIds that contain dynamic sprite components
	std::set<EntityId> dynamicSpriteEntityIds;
	// set of EntityIds that contain static sprite components
	std::set<EntityId> staticSpriteEntityIds;

	bool firstRun = true;

	DistortionGrid distortionGrid;
	ObjectGrid objectGrid;
	RoomGrid roomGrid;
	LevelGenerator levelGenerator;
	AStarGrid aStarGrid;

	// render texture for static objects in the world, I.E. objects that don't move.
	sf::RenderTexture worldTextureStatic;
	// render texture for dynamic objects in the world, I.E. objects that do move, or even change sprite.
	sf::RenderTexture worldTextureDynamic;
	
	// texture for the background, drawn to worldTextureStatic every frame
	sf::RenderTexture backgroundTexture;

	void roomGridGenerate();
};

using GameLevelGrid = LevelGrid<GameLevel>;

#endif