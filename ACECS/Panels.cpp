#include "Panels.hpp"

#include "../Include/Common/Math.hpp"
#include "../Include/Common/VectorMath.hpp"
#include "../Include/Game/World/Distortions/WorldDistortionGrid.hpp"
#include "../Include/Game/World/Image Grid/WorldImageGrid.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Input.hpp"

void PanelGameView::panelUpdate() {

	checkModeChange();

	if (mode == Normal) {

	}
}

void PanelGameView::checkModeChange() {
}
