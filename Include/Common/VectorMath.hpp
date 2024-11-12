#ifndef __VECTOR_MATH_H__
#define __VECTOR_MATH_H__

#include "SFML/System.hpp"
#include <cmath>

template <typename T, typename R>
class Vector2Math {
private:
	typedef sf::Vector2<T> Vector2;
public:

	static R lengthSqrd(T x, T y) {
		return (x * x) + (y * y);
	}

	static R lengthSqrd(Vector2 vec) {
		return lengthSqrd(vec.x, vec.y);
	}

	static R length(T x, T y) {
		return sqrt(lengthSqrd(x, y));
	}
	static R length(Vector2 vec) {
		return length(vec.x, vec.y);
	}

	// gets the axis from (aX, aY) to (bX, bY)
	static Vector2 axis(T aX, T aY, T bX, T bY) {
		return Vector2(bX - aX, bY - aY);
	}
	// gets the axis from vecA to vecB
	static Vector2 axis(Vector2 vecA, Vector2 vecB) {
		return axis(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static R distSqrd(T aX, T aY, T bX, T bY) {
		return lengthSqrd(axis(aX, aY, bX, bY));
	}
	static R distSqrd(Vector2 vecA, Vector2 vecB) {
		return distSqrd(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static R dist(T aX, T aY, T bX, T bY) {
		return length(axis(aX, aY, bX, bY));
	}
	static R dist(Vector2 vecA, Vector2 vecB) {
		return dist(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static Vector2 normalize(Vector2 vec) {
		return vec / length(vec);
	}
	static Vector2 normalize(T x, T y) {
		return Vector2(x, y) / length(x, y);
	}

	static Vector2 lengthSet(T x, T y, float len) {
		return normalize(x, y) * len;
	}
	static Vector2 lengthSet(Vector2 vec, float len) {
		return lengthSet(vec.x, vec.y, len);
	}

	// gets the direction from (aX, aY) to (bX, bY)
	static Vector2 dir(T aX, T aY, T bX, T bY) {
		return normalize(axis(aX, aY, bX, bY));
	}
	// gets the direction from vecA to vecB
	static Vector2 dir(Vector2 vecA, Vector2 vecB) {
		return dir(vecA.x, vecA.y, vecB.x, vecB.y);
	}


	static R angle(T aX, T aY, T bX, T bY) {
		auto vecAxis = axis(aX, aY, bX, bY);

		return atan2(vecAxis.y, vecAxis.x);
	}
	static R angle(Vector2 vecA, Vector2 vecB) {
		return angle(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static R dot(T aX, T aY, T bX, T bY) {
		return (aX * bX) + (aY * bY);
	}
	static R dot(Vector2 vecA, Vector2 vecB) {
		return dot(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static R angleDifference(T aX, T aY, T bX, T bY) {

		R dotProduct = dot(aX, aY, bX, bY);
		R combinedLength = length(aX, aY) * length(bX, bY);

		return static_cast<R>(std::acos(dotProduct / combinedLength));
	}
	static R angleDifference(Vector2 vecA, Vector2 vecB) {
		return angleDifference(vecA.x, vecA.y, vecB.x, vecB.y);
	}

	static Vector2 reflect(T dX, T dY, T nX, T nY) {

		R dotProduct = dot(dX, dY, nX, nY);

		T rX = dX - (2 * dotProduct * nX);
		T rY = dY - (2 * dotProduct * nY);

		return Vector2(rX, rY);
	}
	static Vector2 reflect(Vector2 dir, Vector2 normal) {
		return reflect(dir.x, dir.y, normal.x, normal.y);
	}
	template <typename F>
	static Vector2 rotate(T x, T y, F theta) {
		return Vector2(
			(x * T(std::cos(theta))) - (y * T(std::sin(theta))),
			(x * T(std::sin(theta))) + T(y * T(std::cos(theta)))
		);
	}
	template <typename F>
	static Vector2 rotate(Vector2 vector, F theta) {
		return rotate(vector.x, vector.y, theta);
	}
	static Vector2 abs(T x, T y) {
		return Vector2(std::abs(x), std::abs(y));
	}
	static Vector2 abs(Vector2 vector) {
		return abs(vector.x, vector.y);
	}
};

using Vector2fMath = Vector2Math<float, float>;
using Vector2dMath = Vector2Math<double, double>;
using Vector2iMath = Vector2Math<int, float>;

#endif