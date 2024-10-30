#ifndef __PANELS_H__
#define __PANELS_H__

#include <Graphics.hpp>

enum PanelTypes : uint16_t {
	GameView,
};

struct PanelGameView : public Panel {
	using Panel::Panel;

	enum Modes {
		Normal,
		ObjectGridRender,
	};

	Modes mode = Normal;

	void panelUpdate() final;
	
private:
	void checkModeChange();
};


#endif