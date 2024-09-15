#ifndef __NUMBER_GENERATOR_H__
#define __NUMBER_GENERATOR_H__

#include <random>

class NumberGenerator {
protected:
	// used to seed generator
	std::random_device generatorSeed;

	uint32_t getState() {
		uint32_t x = state;

		x ^= x << 13;
		x ^= x >> 17;
		x ^= x << 5;

		return state = x;
	}

private:
	// random number generator
	uint32_t state = generatorSeed();
};

template <typename T>
class RealNumberGenerator : public NumberGenerator {
public:

	RealNumberGenerator() :
		NumberGenerator()
	{}

	float get() {
		return float(getState()) / UINT32_MAX;
	}

	float getUnder(T max) {
		return get() * max;
	}

	float getRange(T min, T max) {
		return min + get() * (max - min);
	}

	float getRange(T width) {
		return getRange(-width * 0.5f, width * 0.5f);
	}
};

template <typename T>
class RNG {
private:
	static RealNumberGenerator<T>* generator;
public:

	static void initialize() {
		generator = new RealNumberGenerator<T>();
	}

	static T get() {
		return generator->get();
	}

	static float getUnder(T max) {
		generator->getUnder(max);
	}

	static float getRange(T min, T max) {
		return generator->getRange(min, max);
	}

	static float getRange(T width) {
		return generator->getRange(width);
	}

	static float getFullRange(T width) {
		return generator->getRange(static_cast<T>(2.f) * width);
	}

	static bool probability(float value) {
		return value > get();
	}
};

using RNGf = RNG<float>;

template <typename T>
RealNumberGenerator<T>* RNG<T>::generator = nullptr;

template <typename T>
class IntegerNumberGenerator : public NumberGenerator {
public:
	IntegerNumberGenerator() :
		NumberGenerator()
	{}

	T getUnder(T max) {

		return getState() % max;
	}

	T getRange(T min, T max) {
		return (getState() % (max - min)) + min;
	}
};

template <typename T>
class RNGi {
private:
	static IntegerNumberGenerator<T> generator;

public:
	static T getUnder(T max) {
		return generator.getUnder(max);
	}

	static T getRange(T min, T max) {
		return generator.getRange(min, max);
	}
};

template <typename T>

IntegerNumberGenerator<T> RNGi<T>::generator;

using RNGi16 = RNGi<int16_t>;
using RNGi32 = RNGi<int32_t>;
using RNGi64 = RNGi<int64_t>;
using RNGu8 = RNGi<uint8_t>;
using RNGu16 = RNGi<uint16_t>;
using RNGu32 = RNGi<uint32_t>;
using RNGu64 = RNGi<uint64_t>;

template <typename T>
class RNGPool {
private:
	static inline std::vector<T*> pools;
	static inline std::vector<uint32_t> poolSizes;
	static inline uint16_t poolCount = 0;
public:
	// creates a new RNG pool and returns it's pool index
	static uint16_t createNewPool(uint32_t size) {
		pools.push_back(new T[size]);
		poolSizes.push_back(size);
		poolCount++;
		return poolCount - 1;
	}

	static void setPoolIndexValue(uint16_t poolIndex, uint32_t valueIndex, T value) {
		pools[poolIndex][valueIndex] = value;
	}

	static void fillPool(uint16_t poolIndex) {
		for (uint32_t i = 0; i < poolSizes[poolIndex]; i++) {
			setPoolIndexValue(poolIndex, i, T(RNGf::get()));
		}
	}

	static void fillPoolRange(uint16_t poolIndex, T width) {
		for (uint32_t i = 0; i < poolSizes[poolIndex]; i++) {
			setPoolIndexValue(poolIndex, i, T(RNGf::getRange(width)));
		}
	}

	static void fillPoolRange(uint16_t poolIndex, T min, T max) {
		for (uint32_t i = 0; i < poolSizes[poolIndex]; i++) {
			setPoolIndexValue(poolIndex, i, T(RNGf::getRange(min, max)));
		}
	}

	static void deletePool(uint16_t poolIndex) {
		delete[] pools[poolIndex];
		poolCount -= 1;
	}

	static T getPoolValue(uint16_t poolIndex, uint16_t valueIndex) {
		return pools[poolIndex][valueIndex];
	}
};

using RNGfPool = RNGPool<float>;

#endif