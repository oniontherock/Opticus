#ifndef __GAME_STATES_H__
#define __GAME_STATES_H__

#include "GameState/GameStateHandler.hpp"

enum GameStateTypes : uint16_t {
	Play,
	Pause,
	Win,
	Lose,
};

struct GameStatePlay : public GameState {

	GameStatePlay(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Play;
	}

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;
private:
	bool firstUpdate = false;

	void gameStateStart();
	void levelGenerate();
	void worldClockUpdate();
};

struct GameStatePause : public GameState {

	GameStatePause(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Pause;
	}

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;
};

struct GameStateWin : public GameState {

	GameStateWin(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Win;
	}

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;
};

struct GameStateLose : public GameState {

	GameStateLose(std::vector<GameStateTransition> _transitions, std::vector<PanelName> _panelNames) :
		GameState::GameState(_transitions, _panelNames)
	{
		id = GameStateTypes::Lose;
	}

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void gameStateUpdate() final;

private:
	// cooldown for closing the window
	Cooldown exitCooldown = Cooldown(2.5f);
};

#endif