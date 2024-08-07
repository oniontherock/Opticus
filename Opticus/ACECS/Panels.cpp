#include "Panels.hpp"

#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Graphics/PanelManager.hpp"
#include "Input.hpp"

void PanelGameView::panelUpdate() {

	checkModeChange();

	if (mode == Normal) {
	}
}

void PanelGameView::checkModeChange() {
}
