#include "../ACECS/ACECS.hpp"
#include "Graphics/WindowHolder.hpp"
#include "Input/Input Events/InputManager.hpp"

int main() {


	// setup window
	WindowHolder::windowInitialize(sf::VideoMode(1280, 720), "Window");

	Engine::engineInitialize();

	// run main program loop if window is open
	while (WindowHolder::windowGet().isOpen()) {
		
		Engine::engineInputUpdate(WindowHolder::windowGet());
		Engine::engineUpdate();

		WindowHolder::windowGet().clear(sf::Color::Black);

		Engine::engineDraw(WindowHolder::windowGet());

		WindowHolder::windowGet().display();
	}

	return 0;
}