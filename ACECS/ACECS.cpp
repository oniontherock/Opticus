#include "ACECS.hpp"

#include "../Include/Simulation/WorldImageGrid.hpp"
#include "../Include/Simulation/Distortions/WorldDistortionGrid.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "GameState.hpp"
#include "GameStates.hpp"
#include "Input.hpp"
#include "Panels.hpp"
#include "World.hpp"
#include <cmath>

void Engine::inputsRegister() {
	InputInterface::inputRegister("Pause", KeySet{ KeyEvent("Escape", Pressed) });
	
	InputInterface::inputRegister("Move Right", KeySet{ KeyEvent("D", Held), KeyEvent("Right", Held) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Move Up", KeySet{ KeyEvent("W", Held), KeyEvent("Up", Held) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Move Left", KeySet{ KeyEvent("A", Held), KeyEvent("Left", Held) }, InputKeyLogic::Or);
	InputInterface::inputRegister("Move Down", KeySet{ KeyEvent("S", Held), KeyEvent("Down", Held) }, InputKeyLogic::Or);


}

// game states are registered here
void Engine::panelsRegister() {
	using namespace PanelManager;
	panelAdd(PanelTypes::GameView, PanelPtr(new PanelGameView(
		PanelRect(0, 0, 1280, 720), // screen coordinates
		PanelRect(0, 0, 1280, 720), // world coordinates
		sf::Color::Black
	)));
}

// game states are registered here
void Engine::gameStateRegister() {

	GameStateHandler::gameStateAdd(GameStateUniquePtr(new GameStatePlay(
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition(GameStateType::Pause, // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Pause",
				}
			),
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			PanelTypes::GameView,
		}
		)));

	GameStateHandler::gameStateAdd(GameStateUniquePtr(new GameStatePause(
		/// transitions
		// vector of GameStateTransitions, and their inputs
		{
			GameStateTransition(GameStateType::Play, // name of the state to transition to
				/// transition inputs
				// vector of inputs that trigger this transition
				// note the commas after an input name, without commas every name will become a single string
				{
					"Pause",
				}
			),
		},
		/// panels
		// the panels belonging to this GameState,
		// note the commas after every panel name, without commas every name will become a single string
		{
			PanelTypes::GameView,
		}
		)));

	GameStateHandler::gameStatesAddedStatesFinalize();
	GameStateHandler::gameStateForceSet(GameStateTypes::Play);
}

// initialize the ACECS engine by registering all inputs, initializing the ECS module, and registering game states.
// of course, certain modules do not have to be initialized if the user does not want them to be
void Engine::engineInitialize() {

	GameLevelGrid::levelGridInitialize(1, 1, 1);
	GameLevelGrid::levelAdd(new GameLevel(0, 0, 0), true);

	inputsRegister();
	ECSRegistry::ECSInitialize();
	panelsRegister();
	gameStateRegister();

	WorldImageGrid::worldTextureGridInitialize(1, 1, 1280, 720);
	
	distortionFunctionsInitialize();
	WorldDistortionGrid::worldDistortionGridInitialize(1280, 720, 1, 1);
}
// updates the engines input
void Engine::engineInputUpdate(sf::RenderWindow& window) {
	InputInterface::inputUpdate();

	InputInterface::eventsProcess(window);
}
// update certain modules of the engine, like the input system, and the game state.
// note that certain modules, like the ECS system, are updated inside the GameStateHandler,
// because you don't want to update the ECS system if the GameState is currently paused, for example.
void Engine::engineUpdate() {
	GameStateHandler::gameStateProcess();
}
void Engine::engineDraw(sf::RenderWindow& renderWindowMain) {

	auto& gameStatePanels = GameStateHandler::gameStateGetPanels();

	for (uint16_t i = 0; i < gameStatePanels.size(); i++) {
		PanelManager::panelGet(gameStatePanels[i]).panelDrawObjects();
		PanelManager::panelGet(gameStatePanels[i]).panelRender(renderWindowMain);
		PanelManager::panelGet(gameStatePanels[i]).panelClear();
	}
}
// terminates certain engine modules, like the ECS or GameStateHandler
void Engine::engineTerminate() {
	ECSRegistry::ECSTerminate();
	GameStateHandler::gameStatesAllTerminate();
	PanelManager::panelManagerTerminate();
}
