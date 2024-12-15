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

void EntityComponents::ComponentViewFollow::save(std::ofstream& str) {
	str << panelViewToFollow;
}
void EntityComponents::ComponentViewFollow::load(std::ifstream& str) {
	str >> panelViewToFollow;
}

void EntityComponents::ComponentObjectGridInhabiterRadius::save(std::ofstream& str) {
	str << radius;
	str << positionPrev;
}
void EntityComponents::ComponentObjectGridInhabiterRadius::load(std::ifstream& str) {
	str >> radius;
	str >> positionPrev;
}

void EntityComponents::ComponentObjectVision::save(std::ofstream& str) {
	str << objectVision;
	str << cooldownVisionUpdate;
}
void EntityComponents::ComponentObjectVision::load(std::ifstream& str) {
	str >> objectVision;
	str >> cooldownVisionUpdate;
}

void EntityComponents::ComponentObjectMemory::save(std::ofstream& str) {
	str << objectMemoryHolder;
}
void EntityComponents::ComponentObjectMemory::load(std::ifstream& str) {
	str >> objectMemoryHolder;
}

void EntityComponents::ComponentRotation::save(std::ofstream& str) {
	str << rotation;
}
void EntityComponents::ComponentRotation::load(std::ifstream& str) {
	str >> rotation;
}

void EntityComponents::ComponentActorData::save(std::ofstream& str) {
	str << actorDataHolder;
}
void EntityComponents::ComponentActorData::load(std::ifstream& str) {
	str >> actorDataHolder;
}

void EntityComponents::ComponentActorBlackboard::save(std::ofstream&) {

}
void EntityComponents::ComponentActorBlackboard::load(std::ifstream&) {
	updateFunc = [](Entity& actor, ActorBlackboard& actorBlackboard) {

		auto objectsSeen = actorBlackboard.dataGet<ObjectIdVector>("ObjectsSeen");

		if (objectsSeen[uint16_t(ObjectType::Player)].size() > 0) {

			EntityId playerId = objectsSeen[uint16_t(ObjectType::Player)][0];

			// set this actor's LeaderId to the playerId.
			actorBlackboard.dataSet("LeaderId", playerId);
		}

		if (actorBlackboard.dataHas("Orders")) {

			OrderVector* orders = actorBlackboard.dataGet<OrderVector*>("Orders");

			OrderSubVector& ordersDismiss = orders->at(uint16_t(OrderType::OrdersDismiss));

			// vector of entityIds for the orders to dismiss
			std::set<EntityId> ordersFromDismissVector;

			for (uint16_t i = 0; i < ordersDismiss.size(); i++) {

				if (!OrderHandler::actorOrderShouldAccept(actor.Id, actorBlackboard, ordersDismiss[i])) continue;

				ordersFromDismissVector.insert(ordersDismiss[i].dataGet<EntityId>("OrderFromId"));
			}

			for (uint16_t i = 0; i < uint16_t(OrderType::SIZE); i++) {
				OrderSubVector& orderSubVector = orders->at(i);

				for (int16_t j = int16_t(orderSubVector.size() - 1); j >= 0; j--) {
					// skip if the order is not one of the orders we should dismiss
					if (!OrderHandler::actorOrderIsFromActorSetToActor(ordersFromDismissVector, actor.Id, orderSubVector[j])) continue;

					orderSubVector.erase(orderSubVector.begin() + j);
				}
			}
		}
		};
}

void EntityComponents::ComponentActorMovementHandler::save(std::ofstream& str) {
	str << movementType;
}
void EntityComponents::ComponentActorMovementHandler::load(std::ifstream& str) {
	str >> movementType;
}

void EntityComponents::ComponentActorStateManager::save(std::ofstream& str) {
	str << stateManager;
}
void EntityComponents::ComponentActorStateManager::load(std::ifstream& str) {
	str >> stateManager;
}

void EntityComponents::ComponentAStarPathHolder::save(std::ofstream& str) {
	str << endTarget;
	str << path;
	str << pathTarget;
	str << updateCooldown;
}
void EntityComponents::ComponentAStarPathHolder::load(std::ifstream& str) {
	str >> endTarget;
	str >> path;
	str >> pathTarget;
	str >> updateCooldown;
}

void EntityComponents::ComponentAudioPlayOnMove::save(std::ofstream& str) {
	str << pitchOffset;
	str << pitchOffset;
	str << posLast;
	str << soundName;
	str << volumeOffset;
}
void EntityComponents::ComponentAudioPlayOnMove::load(std::ifstream& str) {
	str >> pitchOffset;
	str >> pitchOffset;
	str >> posLast;
	str >> soundName;
	str >> volumeOffset;
}

void EntityComponents::ComponentEventOnObjectNear::save(std::ofstream& str) {
	str << radius;
}
void EntityComponents::ComponentEventOnObjectNear::load(std::ifstream& str) {
	str >> radius;
}

void EntityComponents::ComponentObjectTypeAssigner::save(std::ofstream& str) {
	str << objectType;
}
void EntityComponents::ComponentObjectTypeAssigner::load(std::ifstream& str) {
	str >> objectType;
}

#pragma endregion
#pragma region Distortions
void Distortions::DistortionHeadingMultiply::save(std::ofstream& str) {
	str << multiplier;
}
void Distortions::DistortionHeadingMultiply::load(std::ifstream& str) {
	str >> multiplier;
}
void Distortions::DistortionPositionSet::save(std::ofstream& str) {
	str << point;
}
void Distortions::DistortionPositionSet::load(std::ifstream& str) {
	str >> point;
}


#pragma endregion

std::ofstream& operator<< (std::ofstream& str, UtilityStateManager& item) {

	str << item.stateActiveInd;
	str << item.stateScores;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, UtilityStateManager& item) {

	str >> item.stateActiveInd;
	str >> item.stateScores;

	item.statesVector = StateVector{
		StateSharedPtr(new UtilityStates::StateIdle()),
		StateSharedPtr(new UtilityStates::StateActorFollow()),
		StateSharedPtr(new UtilityStates::StatePointGoTo())
	};

	return str;
}

std::ofstream& operator<< (std::ofstream& str, ActorDataHolder& item) {
	str << item.emotionsVector;
	str << item.traitsVector;

	return str;
}
std::ifstream& operator>> (std::ifstream& str, ActorDataHolder& item) {
	str >> item.emotionsVector;
	str >> item.traitsVector;

	item.emotionUpdateFunction = [](const ActorBlackboard&, ActorDataHolder&) {};

	return str;
}

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

std::ofstream& operator<< (std::ofstream& str, ObjectVision& item) {
	str << item.castAngle;
	str << item.castPosition;

	size_t objectSeenLength = item.objectsSeenVector.size();
	str << objectSeenLength;

	for (uint32_t x = 0; x < objectSeenLength; x++) {
		size_t objectSeenSubLength = item.objectsSeenVector[x].size();
		str << objectSeenSubLength;

		for (uint32_t y = 0; y < objectSeenSubLength; y++) {
			str << item.objectsSeenVector[x][y];
		}
	}
	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectVision& item) {
	str >> item.castAngle;
	str >> item.castPosition;

	size_t objectSeenLength;
	str >> objectSeenLength;
	item.objectsSeenVector.resize(objectSeenLength);

	for (uint32_t x = 0; x < objectSeenLength; x++) {
		size_t objectSeenSubLength;
		str >> objectSeenSubLength;
		item.objectsSeenVector[x].resize(objectSeenSubLength);

		for (uint32_t y = 0; y < objectSeenSubLength; y++) {
			str >> item.objectsSeenVector[x][y];
		}
	}

	return str;
}

std::ofstream& operator<< (std::ofstream& str, ObjectMemoryHolder& item) {
	str << item.memoryMaxLife;
	str << item.updateTimeLast;

	size_t objectMemoriesLength = item.objectMemoriesVector.size();
	str << objectMemoriesLength;

	for (uint32_t x = 0; x < objectMemoriesLength; x++) {
		size_t objectMemoriesSubLength = item.objectMemoriesVector[x].size();
		str << objectMemoriesSubLength;

		for (uint32_t y = 0; y < objectMemoriesSubLength; y++) {
			str << item.objectMemoriesVector[x][y].first;
			str << item.objectMemoriesVector[x][y].second;
		}
	}

	return str;
}
std::ifstream& operator>> (std::ifstream& str, ObjectMemoryHolder& item) {
	str >> item.memoryMaxLife;
	str >> item.updateTimeLast;

	size_t objectMemoriesLength;
	str >> objectMemoriesLength;
	item.objectMemoriesVector.resize(objectMemoriesLength);

	for (uint32_t x = 0; x < objectMemoriesLength; x++) {
		size_t objectMemoriesSubLength;
		str >> objectMemoriesSubLength;
		item.objectMemoriesVector[x].resize(objectMemoriesSubLength);

		for (uint32_t y = 0; y < objectMemoriesSubLength; y++) {
			str >> item.objectMemoriesVector[x][y].first;
			str >> item.objectMemoriesVector[x][y].second;
		}
	}

	return str;
}
