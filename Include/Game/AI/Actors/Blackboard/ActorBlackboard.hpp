#ifndef __ACTOR_BLACKBOARD_H__
#define __ACTOR_BLACKBOARD_H__

#include <any>
#include <string>
#include <unordered_map>

// variant of data types that can be contained in an BlackboardDataMap.
typedef std::any BlackboardDataType;
// key of an BlackboardDataType, used in BlackboardDataMap.
typedef const char* BlackboardDataKey;
// map of names and data.
typedef std::unordered_map<BlackboardDataKey, BlackboardDataType> BlackboardDataMap;

struct ActorBlackboard {
	// removes the element from the blackboardDataMap whose key is dataKey
	void dataRemove(BlackboardDataKey dataKey);
	// gets the data associated with dataKey, T is the type of the data that is being obtained.
	template <typename T>
	T dataGet(BlackboardDataKey dataKey) const {
		return std::any_cast<T>(blackboardDataMap.at(dataKey));
	}
	// sets the value of dataKey to value.
	template <typename T>
	void dataSet(BlackboardDataKey dataKey, T value) {
		blackboardDataMap[dataKey] = value;
	}
	// returns whether the specified dataKey exists in the blackboardDataMap.
	bool dataHas(BlackboardDataKey dataKey) const;
private:

	// map of all data contained in the blackboard
	BlackboardDataMap blackboardDataMap;
};

#endif
