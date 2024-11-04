#include "ActorDataHolder.hpp"
#include <Auxiliary/ConsoleHandler.hpp>

ActorDataHolder::ActorDataHolder() {
	traitsInitialize();
	emotionsInitialize();
}
ActorDataHolder::ActorDataHolder(TraitVector _traitsVector, EmotionVector _emotionsVector) {
	traitsInitialize(_traitsVector);
	emotionsInitialize(_emotionsVector);
}

ScaleValue ActorDataHolder::traitGet(uint8_t trait) {
	return traitsVector[trait];
}
ScaleValue ActorDataHolder::traitGet(ActorTrait trait) {
	return traitGet(uint8_t(trait));
}

void ActorDataHolder::traitSet(uint8_t trait, ScaleValue value) {
	traitsVector[trait] = value;
}
void ActorDataHolder::traitSet(ActorTrait trait, ScaleValue value) {
	traitSet(uint8_t(trait), value);
}


ScaleValue ActorDataHolder::emotionGet(uint8_t emotion) {
	return emotionsVector[emotion];
}
ScaleValue ActorDataHolder::emotionGet(ActorEmotion emotion) {
	return emotionGet(uint8_t(emotion));
}

void ActorDataHolder::emotionSet(uint8_t emotion, ScaleValue value) {
	emotionsVector[emotion] = value;

}
void ActorDataHolder::emotionSet(ActorEmotion emotion, ScaleValue value) {
	emotionSet(uint8_t(emotion), value);
}

void ActorDataHolder::emotionIncrement(uint8_t emotion, int8_t value) {
	emotionsVector[emotion] += value;

}
void ActorDataHolder::emotionIncrement(ActorEmotion emotion, int8_t value) {
	emotionIncrement(uint8_t(emotion), value);
}


void ActorDataHolder::dataRemove(const ActorDataName& dataName) {
	actorDataMap.erase(dataName);
}


void ActorDataHolder::traitsInitialize() {
	traitsVector.resize(uint8_t(ActorTrait::SIZE));

	for (uint8_t i = 0; i < uint8_t(ActorTrait::SIZE); i++) {
		traitsVector[i] = 0;
	}
}
void ActorDataHolder::traitsInitialize(TraitVector _traitsVector) {
	try {

		if (_traitsVector.size() < uint8_t(ActorTrait::SIZE)) {
			throw "Exception: vector too small";
		}
		else {
			throw "Exception: vector too large";
		}

		traitsVector = _traitsVector;
	}
	catch (std::string error) {
		ConsoleHandler::consolePrintErr("Failure to initialize an ActorDataHolder's traits: " + error);
	}
}
void ActorDataHolder::emotionsInitialize() {
	emotionsVector.resize(uint8_t(ActorEmotion::SIZE));

	for (uint8_t i = 0; i < uint8_t(ActorEmotion::SIZE); i++) {
		emotionsVector[i] = 0;
	}
}
void ActorDataHolder::emotionsInitialize(EmotionVector _emotionsVector) {
	try {

		if (_emotionsVector.size() < uint8_t(ActorEmotion::SIZE)) {
			throw "Exception: vector too small";
		}
		else {
			throw "Exception: vector too large";
		}

		emotionsVector = _emotionsVector;
	}
	catch (std::string error) {
		ConsoleHandler::consolePrintErr("Failure to initialize an ActorDataHolder's emotions: " + error);
	}
}