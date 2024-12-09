#include "SaveOperations.hpp"
#include <Saving/SaveDirector.hpp>
#include <ECS/Entities/EntityManager.hpp>
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
	
	//// save entity count
	//size_t entitiesCount = EntityManager::entitiesVector.size();
	//SaveHandler::objectSave(entitiesCount);
	//// save entities
	//for (uint32_t i = 0; i < EntityManager::entitiesVector.size(); i++) {
	//	SaveHandler::objectSave<Entity&>(EntityManager::entitiesVector[i]);
	//}
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
	//// load entity count
	//size_t entitiesCount;
	//SaveHandler::objectLoad(entitiesCount);
	//// load entities
	//for (uint32_t i = 0; i < entitiesCount; i++) {
	//	Entity entityCur;
	//	SaveHandler::objectLoad(entityCur);

	//	EntityManager::entitiesVector[entityCur.Id] = entityCur;

	//}
}
