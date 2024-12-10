#include "SaveOperations.hpp"


std::ofstream& operator<< (std::ofstream& str, std::string& item) {
	size_t itemLen = item.size();
	str << itemLen;

	str.write(item.data(), itemLen * sizeof(char));

	return str;
}
std::ifstream& operator>> (std::ifstream& str, std::string& item) {
	size_t itemLen;
	str >> itemLen;
	item.resize(itemLen);

	str.read(item.data(), itemLen * sizeof(char));

	return str;
}


std::ofstream& operator<< (std::ofstream& str, GameLevel& item) {

	str << item.levelSize;
	str << item.idPlayer;
	str << item.dynamicSpriteEntityIds;
	str << item.staticSpriteEntityIds;
	str << item.firstRun;
	//str << item.distortionGrid;
	str << item.objectGrid;
	//str << item.roomGrid;
	//str << item.levelGenerator;
	str << item.aStarGrid;
	//str << item.worldTextureStatic;
	//str << item.worldTextureDynamic;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, GameLevel& item) {

	str >> item.levelSize;
	str >> item.idPlayer;
	str >> item.dynamicSpriteEntityIds;
	str >> item.staticSpriteEntityIds;
	str >> item.firstRun;
	//str >> item.distortionGrid;
	str >> item.objectGrid;
	//str >> item.roomGrid;
	//str >> item.levelGenerator;
	str >> item.aStarGrid;
	//str >> item.worldTextureStatic;
	//str >> item.worldTextureDynamic;

	return str;
}

std::ofstream& operator<< (std::ofstream& str, AStarCell& item) {
	str << item.cellPositionGrid;
	str << item.cellPositionWorld;
	str << item.costF;
	str << item.costG;
	str << item.costH;
	str << item.valid;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, AStarCell& item) {
	str >> item.cellPositionGrid;
	str >> item.cellPositionWorld;
	str >> item.costF;
	str >> item.costG;
	str >> item.costH;
	str >> item.valid;

	return str;
}

std::ofstream& operator<< (std::ofstream& str, AStarGrid& item) {
	str << item.gridSize;
	str << item.cellCount;
	str << item.cellSize;
	str << item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str << item.cells[x][y];
		}
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, AStarGrid& item) {
	str >> item.gridSize;
	str >> item.cellCount;
	str >> item.cellSize;
	str >> item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str >> item.cells[x][y];
		}
	}

	return str;
}

std::ofstream& operator<< (std::ofstream& str, ObjectCell& item) {
	str << item.idsSet;
	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectCell& item) {
	str >> item.idsSet;
	return str;
}

std::ofstream& operator<< (std::ofstream& str, ObjectGrid& item) {
	str << item.gridSize;
	str << item.cellCount;
	str << item.cellSize;
	str << item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str << item.cells[x][y];
		}
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectGrid& item) {
	str >> item.gridSize;
	str >> item.cellCount;
	str >> item.cellSize;
	str >> item.gridSizeFull;

	for (uint32_t x = 0; x < item.gridSize.x; x++) {
		for (uint32_t y = 0; y < item.gridSize.y; y++) {
			str >> item.cells[x][y];
		}
	}

	return str;
}

std::ofstream& operator<< (std::ofstream& str, Entity& item) {
	str << item.updateType;
	str << item.levelId;

	for (EntityComponents::ComponentTypeID componentIdCur = 0; componentIdCur < EntityComponents::totalComponents; componentIdCur++) {
		if (item.entityComponentHasAtIndex(componentIdCur)) {
			str << componentIdCur;
			item.entityComponentGetAtIndex(componentIdCur)->save(str);
		}
	}
	// put in super high value so that the component loading loop exits
	str << EntityComponents::totalComponents;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, Entity& item) {
	str >> item.updateType;
	str >> item.levelId;

	EntityComponents::ComponentTypeID componentIdCur;
	str >> componentIdCur;
	do {

		// get the component of the current type that the entity saved.
		EntityComponents::Component* componentToDuplicate = EntityComponents::componentsAll[componentIdCur].get();
		// duplicate the component that the entity saved
		ComponentUniquePtr componentDuplicated = Duplicatable::duplicateAndConvertToType<EntityComponents::Component>(componentToDuplicate);
		// raw pointer to the duplicate that was duplicated
		EntityComponents::Component* componentDuplicatedRaw = componentDuplicated.get();
		// release unique ptr to duplicated component
		componentDuplicated.release();

		// load the data that the entity saved for this component
		componentDuplicatedRaw->load(str);

		// add the duplicated type to the entity
		item.entityComponentAddAtIndex(componentDuplicatedRaw, componentIdCur);

		str >> componentIdCur;
	} while (componentIdCur < EntityComponents::totalComponents);

	return str;
}

#pragma region Components
void EntityComponents::ComponentPosition::save(std::ofstream& str) {
	str << position;
}
void EntityComponents::ComponentPosition::load(std::ifstream& str) {
	str >> position;
}

void EntityComponents::ComponentSprite::save(std::ofstream& str) {
	str << fileName;
	str << fileExtension;
}
void EntityComponents::ComponentSprite::load(std::ifstream& str) {
	str >> fileName;
	str >> fileExtension;
	textureInitialize();
}

void EntityComponents::ComponentMoveByInput::save(std::ofstream& str) {
	str << moveSpeed;
}
void EntityComponents::ComponentMoveByInput::load(std::ifstream& str) {
	str >> moveSpeed;
}

void EntityComponents::ComponentRotateToMouse::save(std::ofstream& str) {
	str << turnSpeed;
}
void EntityComponents::ComponentRotateToMouse::load(std::ifstream& str) {
	str >> turnSpeed;
}

void EntityComponents::ComponentVisionCasterHolder::save(std::ofstream& str) {
	str << visionCaster;
	str << updateCooldown;
	str << doUpdate;
}
void EntityComponents::ComponentVisionCasterHolder::load(std::ifstream& str) {
	str >> visionCaster;
	str >> updateCooldown;
	str >> doUpdate;
}
void EntityComponents::ComponentMemoryVision::save(std::ofstream& str) {
	str << cameraMovedAmountTotal;
	str << memoryHolder;
}
void EntityComponents::ComponentMemoryVision::load(std::ifstream& str) {
	str >> cameraMovedAmountTotal;
	str >> memoryHolder;
}

#pragma endregion


std::ofstream& operator<< (std::ofstream& str, VisionCaster& item) {
	str << item.castPosition;
	str << item.castAngle;
	str << item.cameraCenterGlobal;
	str << item.cameraCenterLocal;
	str << item.visionImageCenter;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, VisionCaster& item) {
	str >> item.castPosition;
	str >> item.castAngle;
	str >> item.cameraCenterGlobal;
	str >> item.cameraCenterLocal;
	str >> item.visionImageCenter;

	return str;
}
std::ofstream& operator<< (std::ofstream& str, MemoryHolderVision& item) {
	str << item.blurCooldown;
	str << item.memoryMovementTotal;
	str << item.timeOfLastBlur;
	str << item.memorySize;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, MemoryHolderVision& item) {
	str >> item.blurCooldown;
	str >> item.memoryMovementTotal;
	str >> item.timeOfLastBlur;
	str >> item.memorySize;

	item.textureMemoryInitialize();

	return str;
}

std::ofstream& operator<< (std::ofstream& str, Cooldown& item) {
	str << item.target;
	str << item.value;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, Cooldown& item) {
	str >> item.target;
	str >> item.value;

	return str;
}