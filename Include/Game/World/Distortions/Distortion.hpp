#ifndef __DISTORTION_H__
#define __DISTORTION_H__

#include <Auxiliary/Cooldown.hpp>
#include <Auxiliary/Math.hpp>
#include <Auxiliary/TimeHandler.hpp>
#include <Auxiliary/VectorMath.hpp>
#include <cmath>
#include <ECS/Duplicatable/Duplicatable.hpp>
#include <functional>
#include <queue>


namespace Distortions {

	// time in milliseconds since epoch at which the distortion is removed
	typedef uint32_t DistortionDeathTime;

	struct Distortion : public Duplicatable {

		virtual ~Distortion() = default;

		virtual void distortionApplyToRay(sf::Vector2f&, sf::Vector2f&) {};

		virtual void save(std::ofstream& str);
		virtual void load(std::ifstream& str);

		DistortionDeathTime deathTime = 0;
	};

	typedef std::unique_ptr<Distortion> DistortionUniquePtr;
	typedef std::shared_ptr<Distortion> DistortionSharedPtr;
}

#endif