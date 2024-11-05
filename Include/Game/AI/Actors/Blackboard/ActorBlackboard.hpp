#ifndef __ACTOR_BLACKBOARD_H__
#define __ACTOR_BLACKBOARD_H__

#include <any>
#include <string>
#include <unordered_map>

// variant of data types that can be contained in an BlackboardDataMap.
typedef std::any BlackboardDataType;
// name of an BlackboardDataType, used in BlackboardDataMap.
typedef std::string BlackboardDataName;
// map of names and data.
typedef std::unordered_map<BlackboardDataName, BlackboardDataType> BlackboardDataMap;

struct ActorBlackboard {
	// adds new data of type T with name dataName.
	template <typename T>
	void dataAdd(const BlackboardDataName& dataName, T data) {
		blackboardDataMap[dataName] = BlackboardDataType(data);
	}
	// removes the element from the blackboardDataMap whose key is dataName
	void dataRemove(const BlackboardDataName& dataName);
	// gets the data associated with dataName, T is the type of the data that is being obtained.
	template <typename T>
	T dataGet(const BlackboardDataName& dataName) {
		return std::any_cast<T>(blackboardDataMap[dataName]);
	}
	// sets the data associated with dataName to value.
	template <typename T>
	void dataSet(const BlackboardDataName& dataName, T value) {
		blackboardDataMap[dataName] = value;
	}

private:

	// map of all data contained in the blackboard
	BlackboardDataName blackboardDataMap;
};

#endif
