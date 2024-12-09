#include "SaveOperations.hpp"


std::ofstream& operator<< (std::ofstream& str, GameLevel& item) {

	str << item.levelSize;
	str << item.idPlayer;
	str << item.dynamicSpriteEntityIds;
	str << item.staticSpriteEntityIds;
	str << item.firstRun;
	//str << item.distortionGrid;
	str << item.objectGrid;
	//str << item.roomGrid;
	//str << item.levelGenerator;
	str << item.aStarGrid;
	str << item.worldTextureStatic;
	str << item.worldTextureDynamic;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, GameLevel& item) {

	str >> item.levelSize;
	str >> item.idPlayer;
	str >> item.dynamicSpriteEntityIds;
	str >> item.staticSpriteEntityIds;
	str >> item.firstRun;
	//str >> item.distortionGrid;
	str >> item.objectGrid;
	//str >> item.roomGrid;
	//str >> item.levelGenerator;
	str >> item.aStarGrid;
	str >> item.worldTextureStatic;
	str >> item.worldTextureDynamic;

	return str;
}

std::ofstream& operator<< (std::ofstream& str, AStarCell& item) {
	str << item.cellPositionGrid;
	str << item.cellPositionWorld;
	str << item.costF;
	str << item.costG;
	str << item.costH;
	str << item.valid;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, AStarCell& item) {
	str >> item.cellPositionGrid;
	str >> item.cellPositionWorld;
	str >> item.costF;
	str >> item.costG;
	str >> item.costH;
	str >> item.valid;

	return str;
}

std::ofstream& operator<< (std::ofstream& str, AStarGrid& item) {
	str << item.gridSize;
	str << item.cellCount;
	str << item.cellSize;
	str << item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str << item.cells[x][y];
		}
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, AStarGrid& item) {
	str >> item.gridSize;
	str >> item.cellCount;
	str >> item.cellSize;
	str >> item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str >> item.cells[x][y];
		}
	}

	return str;
}

std::ofstream& operator<< (std::ofstream& str, ObjectCell& item) {
	str << item.idsSet;
	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectCell& item) {
	str >> item.idsSet;
	return str;
}

std::ofstream& operator<< (std::ofstream& str, ObjectGrid& item) {
	str << item.gridSize;
	str << item.cellCount;
	str << item.cellSize;
	str << item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str << item.cells[x][y];
		}
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectGrid& item) {
	str >> item.gridSize;
	str >> item.cellCount;
	str >> item.cellSize;
	str >> item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str >> item.cells[x][y];
		}
	}

	return str;
}

std::ofstream& operator<< (std::ofstream& str, Entity& item) {
	str << item.Id;
	str << item.updateType;
	str << item.levelId;
	return str;
}
std::ifstream& operator>> (std::ifstream& str, Entity& item) {
	str >> item.Id;
	str >> item.updateType;
	str >> item.levelId;
	return str;
}

