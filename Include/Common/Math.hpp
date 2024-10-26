#ifndef __MATH_H__
#define __MATH_H__

#include <cmath>

template <typename T>
class Math {
public:
	static constexpr T PI = T(3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679);
	static constexpr T TAU = T(6.283185307179586476925286766559005768394338798750216419498891846156328125724179972560696506842341359);

	static bool approxEquals(T a, T b, float epsilon) {
		return std::abs(a - b) < epsilon;
	}
	static int sign(T v) {
		return (T(0) < v) - (v < T(0));
	}
};

using Mathf = Math<float>;
using Mathd = Math<double>;

#endif