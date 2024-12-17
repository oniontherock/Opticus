#ifndef __PANELS_H__
#define __PANELS_H__

#include <Graphics.hpp>
#include "GameLevel.hpp"

enum PanelTypes : uint16_t {
	GameView,
	WinScreen,
	LoseScreen,
};

struct PanelGameView : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
		ObjectGridRender,
		PathsRender,
	};

	Modes mode = Normal;

	void panelUpdate() final;

private:
	void checkModeChange();

	// draws all static objects.
	void staticDraw(GameLevel* levelActive);
	// draws all dynamic objects
	void dynamicDraw(GameLevel* levelActive);
	// draws player related stuff like vision and memory
	void playerDraw(GameLevel* levelActive);
	// draws all hud related elements
	void hudDraw(GameLevel* levelActive);
};

struct PanelWinScreen : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void panelUpdate() final;

private:
	void checkModeChange();

	// draws background of win screen
	void backgroundDraw();
	// draws win text
	void textDraw();
};

struct PanelLoseScreen : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
	};

	Modes mode = Normal;

	void panelUpdate() final;

private:
	void checkModeChange();

	// draws background of win screen
	void backgroundDraw();
	// draws win text
	void textDraw();
};

#endif