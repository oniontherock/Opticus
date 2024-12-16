#ifndef __DISTORTION_REGISTRY_H__
#define __DISTORTION_REGISTRY_H__

#include "Distortion.hpp"
#include <fstream>

namespace Distortions {

	void distortionsRegister();

	struct DistortionHeadingMultiply final : Distortion {

		DistortionHeadingMultiply() {
			multiplier = 0.f;
		}
		DistortionHeadingMultiply(float _multiplier) {
			multiplier = _multiplier;
		}

		float multiplier;

		void distortionApplyToRay(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) override;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new DistortionHeadingMultiply(multiplier));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct DistortionPositionSet final : Distortion {

		DistortionPositionSet() {
			point = sf::Vector2f(0, 0);
		}
		DistortionPositionSet(sf::Vector2f _point) {
			point = _point;
		}
		DistortionPositionSet(float x, float y) {
			point = sf::Vector2f(x, y);
		}

		sf::Vector2f point;

		void distortionApplyToRay(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) override;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new DistortionPositionSet(point));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
	struct DistortionPositionOffset final : Distortion {

		DistortionPositionOffset() {
			offset = sf::Vector2f(0, 0);
		}
		DistortionPositionOffset(sf::Vector2f _offset) {
			offset = _offset;
		}
		DistortionPositionOffset(float x, float y) {
			offset = sf::Vector2f(x, y);
		}

		sf::Vector2f offset;

		void distortionApplyToRay(sf::Vector2f& rayHeading, sf::Vector2f& rayPosition) override;

		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new DistortionPositionOffset(offset));
		};

		void save(std::ofstream& str) override;
		void load(std::ifstream& str) override;
	};
}


#endif
