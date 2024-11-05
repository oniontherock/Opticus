#ifndef __ACTOR_DATA_HOLDER_H__
#define __ACTOR_DATA_HOLDER_H__

#include "../Blackboard/ActorBlackboard.hpp"
#include <any>
#include <cstdint>
#include <functional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

// value for scales, like the fear scale that is from 0-100
typedef float ScaleValue;

// variant of data types that can be contained in an actorDataUMap.
typedef std::any ActorDataType;
// key of an ActorDataType, used in actorDataUMap.
typedef const char* ActorDataKey;
// map of keys and data.
typedef std::unordered_map<ActorDataKey, ActorDataType> ActorDataUMap;

enum class ActorTrait : uint8_t {
	Courageousness, // high Courageousness means an actor is less likely to feel fear or panic in times of stress.
	Cooperativeness, // high Cooperativeness means an actor is more likely to follow orders or requests
	Ruthlessness, // high Ruthlessness means an actor is more likely to kill or harm when alternatives are available
	// high Control means an actor is less likely to break under pressure when in high stress situations,
	// this is different from Courageousness, where Courageousness makes an actor less likely to feel fear, Control just changes the way the actor responds to fear.
	// Control makes it so the actor is less likely to, for example, run away just because they are afraid
	Control,
	SIZE, // size of Trait enum
};
typedef std::vector<ScaleValue> TraitVector;

enum class ActorEmotion : uint8_t {
	Fear, // a creatures level of fear
	SIZE, // size of Emotion enum
};
typedef std::vector<ScaleValue> EmotionVector;

// holds data about actors, like their traits and emotions, can be inherited so custom data like HP or speed can be added.
struct ActorDataHolder {
	
	typedef std::function<void(const ActorBlackboard& actorBlackboard, ActorDataHolder& actorData)> ActorEmotionUpdateFunction;

	ActorDataHolder();
	ActorDataHolder(ActorEmotionUpdateFunction _emotionUpdateFunction);
	ActorDataHolder(TraitVector _traitsVector, EmotionVector _emotionsVector, ActorEmotionUpdateFunction _emotionUpdateFunction);

	// returns the ScaleValue for the specified trait.
	ScaleValue traitGet(uint8_t trait) const;
	// returns the ScaleValue for the specified trait.
	ScaleValue traitGet(ActorTrait trait) const;
	
	// sets the specified trait's ScaleValue to the value.
	void traitSet(uint8_t trait, ScaleValue value);
	// sets the specified trait's ScaleValue to the value.
	void traitSet(ActorTrait trait, ScaleValue value);


	// returns the ScaleValue for the specified emotion.
	ScaleValue emotionGet(uint8_t emotion) const;
	// returns the ScaleValue for the specified emotion.
	ScaleValue emotionGet(ActorEmotion emotion) const;

	// sets the specified emotion's ScaleValue to the value.
	void emotionSet(uint8_t emotion, ScaleValue value);
	// sets the specified emotion's ScaleValue to the value.
	void emotionSet(ActorEmotion emotion, ScaleValue value);

	// increments the specified emotion by the value, note that the value can be negative.
	void emotionIncrement(uint8_t emotion, ScaleValue value);
	// increments the specified emotion by the value, note that the value can be negative.
	void emotionIncrement(ActorEmotion emotion, ScaleValue value);

	// update the emotions based off the contents of the blackboard using the emotionUpdateFunction
	void emotionsUpdate(const ActorBlackboard& actorBlackboard);

	// removes the element from the actorDataUMap whose key is dataKey
	void dataRemove(ActorDataKey dataKey);
	// gets the data for dataKey, T is the type of the data that is being obtained.
	template <typename T>
	T dataGet(ActorDataKey dataKey) const {
		return std::any_cast<T>(actorDataUMap[dataKey]);
	}
	// sets the value of dataKey to value.
	template <typename T>
	void dataSet(ActorDataKey dataKey, T value) {
		actorDataUMap[dataKey] = value;
	}
	// returns whether the specified dataKey exists in the actorDataUMap.
	bool dataHas(ActorDataKey dataKey) const;
private:

	// vector of traits, trait values range from 0-100
	TraitVector traitsVector;

	// vector of emotions, emotion values range from 0-100
	EmotionVector emotionsVector;

	// map of custom data unique to this actor.
	// this is used for things like HP or speed of an actor, as not every actor will have these states,
	// whereas every actor WILL have all the emotions and traits, so those do not need to be unique.
	ActorDataUMap actorDataUMap;

	// initialize every element in the traitsVector to zero
	void traitsInitialize();
	// initialize the traitsVector with _traitsVector, _traitsVector must be the size of ActorTraits::SIZE.
	void traitsInitialize(TraitVector _traitsVector);
	// initialize every element in the emotionsVector to zero
	void emotionsInitialize();
	// initialize the emotionsVector with _emotionsVector, _emotionsVector must be the size of ActorEmotions::SIZE.
	void emotionsInitialize(EmotionVector _emotionsVector);

	ActorEmotionUpdateFunction emotionUpdateFunction;
};


#endif