#include "Panels.hpp"

#include "../Include/Common/Math.hpp"
#include "../Include/Common/VectorMath.hpp"
#include "../Include/Game/World/Distortions/WorldDistortionGrid.hpp"
#include "../Include/Game/World/Image Grid/WorldImageGrid.hpp"
#include "../Include/Debugging/ObjectGridRenderer.hpp"
#include "ECS/Entities/EntityManager.hpp"
#include "ECSRegistry.hpp"
#include "GameLevel.hpp"
#include "Input.hpp"

void PanelGameView::panelUpdate() {

	checkModeChange();

	if (mode == Normal || mode == ObjectGridRender) {
	}
	if (mode == ObjectGridRender) {
		ObjectGrid& objectGrid = GameLevelGrid::levelGet(0, 0, 0)->objectGrid;

		static ObjectGridRenderer objectGridRenderer;

		objectGridRenderer.objectGridRender(objectGrid);

		sf::Texture objectGridTexture = objectGridRenderer.objectGridTextureGet();

		sf::Sprite objectGridSprite(objectGridTexture);

		objectGridSprite.setScale(objectGrid.cellsGetSize());

		objectDraw(objectGridSprite);
	}
}

void PanelGameView::checkModeChange() {

	if (InputInterface::inputGetActive("Toggle ObjectGrid Rendering")) {
		if (mode != ObjectGridRender) {
			mode = ObjectGridRender;
		}
		else {
			mode = Normal;
		}
	}
}
