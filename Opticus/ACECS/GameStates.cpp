#include "GameStates.hpp"

#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Graphics.hpp"
#include "Panels.hpp"
#include "World/LevelUpdater.hpp"


void GameStatePlay::gameStateUpdate() {
	LevelUpdater::levelsUpdate();
}

void GameStatePause::gameStateUpdate() {
}



