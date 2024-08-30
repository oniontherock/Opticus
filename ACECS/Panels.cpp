#include "Panels.hpp"

#include "../Include/Common/Math.hpp"
#include "../Include/Common/VectorMath.hpp"
#include "../Include/Simulation/Distortions/WorldDistortionGrid.hpp"
#include "../Include/Simulation/Image Grid/WorldImageGrid.hpp"
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
