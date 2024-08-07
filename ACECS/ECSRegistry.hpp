#ifndef __ECS_REGISTRY_H__
#define __ECS_REGISTRY_H__

#include "ECS.hpp"
#include "SFML/Graphics.hpp"
#include <functional>

namespace ECSRegistry {
	void ECSInitialize();
	void ECSTerminate();
}

// avoid having undefined constructor arguments for events or components,
// as it's more convenient to not have to define every event/component all the time.
// whenever you create a new type, ensure you register it in the implementation file of this header

namespace EntityEvents {
	struct EventExample final : public Event {
		std::unique_ptr<Duplicatable> duplicate() override {
			return std::unique_ptr<Duplicatable>(new EventExample());
		};

		EventExample() {};

		int var = 0;

		void clear() final {
			var = 0;
		}
	};
}
namespace EntityComponents {
	struct ComponentExample final : public Component {

		void system(Entity& entity) final;

		ComponentExample() {
			hasSystem = true;
		};

		DUPLICATE_OVERRIDE(ComponentExample);
	};
}

#endif