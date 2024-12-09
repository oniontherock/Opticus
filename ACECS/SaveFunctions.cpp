#include "SaveOperations.hpp"
#include <Saving/SaveDirector.hpp>
#include "GameLevel.hpp"

// this file is for defining the SaveDirector's saving/loading functions for the game specific data.

void SaveDirector::gameDataSave() {
	// save levels
	for (uint16_t x = 0; x < levelGrid.size(); x++) {
		for (uint16_t y = 0; y < levelGrid[x].size(); y++) {
			for (uint16_t z = 0; z < levelGrid[x][y].size(); z++) {

				GameLevel& level = *static_cast<GameLevel*>(levelGrid[x][y][z].get());
				SaveHandler::objectSave<GameLevel&>(level);
			}
		}
	}
}
void SaveDirector::gameDataLoad() {
	// load levels
	for (uint16_t x = 0; x < levelGrid.size(); x++) {
		for (uint16_t y = 0; y < levelGrid[x].size(); y++) {
			for (uint16_t z = 0; z < levelGrid[x][y].size(); z++) {

				GameLevel& level = *static_cast<GameLevel*>(levelGrid[x][y][z].get());
				SaveHandler::objectLoad<GameLevel&>(level);
			}
		}
	}
}
