#include "../ACECS/ACECS.hpp"
#include "../Include/Common/TimeHandler.hpp"
#include "Auxiliary/ConsoleHandler.hpp"
#include "Graphics/WindowHolder.hpp"
#include "Input/Input Events/InputManager.hpp"
#include <iostream>
#include <iostream>
#include "../Include/Common/Math.hpp"
#include <SFML/Window.hpp>

int main() {

	// setup window
	WindowHolder::windowInitialize(sf::VideoMode(1280, 720), "Window");
	//WindowHolder::windowGet().setFramerateLimit(60);

	Engine::engineInitialize();

	uint32_t frames = 0;
	double frameTimer = 0.0;

	float offsetX = 0;
	float offsetY = 0;

	//run main program loop if window is open
	while (WindowHolder::windowGet().isOpen()) {

		TimeHandler::deltaCompute();

		//sf::Texture texture;
		//texture.loadFromFile("Art/Test Image 2.png");

		//sf::Image positions;
		//positions.create(texture.getSize().x / 2.f, texture.getSize().y / 2.f);

		//offsetX += 0.01f * 1.7777777777777777777777777777778f;
		//offsetY += 0.01f / 1.7777777777777777777777777777778f;

		//uint16_t xVal = 0;
		//uint16_t xExp = 0;
		//uint16_t yVal = 0;
		//uint16_t yExp = 0;

		//for (float x = 0; x < positions.getSize().x; x++) {
		//	for (float y = 0; y < positions.getSize().y; y++) {

		//		for (int16_t i = 1; i <= 255; i++) {

		//			float val = round((x + offsetX)) / i;

		//			if (Mathf::approxEquals(val, round(val), 0.01f)) {
		//				xVal = uint16_t(val);
		//				xExp = i;
		//				break;
		//			}
		//		}
		//		for (int16_t i = 1; i <= 255; i++) {

		//			float val = round((y + offsetY)) / i;

		//			if (Mathf::approxEquals(val, round(val), 0.01f)) {
		//				yVal = uint16_t(val);
		//				yExp = i;
		//				break;
		//			}
		//		}

		//		positions.setPixel(x, y, sf::Color(xVal, xExp, yVal, yExp));
		//	}
		//}
		//sf::Texture posTex;
		//posTex.loadFromImage(positions);
		//
		//sf::Shader shader;
		//shader.loadFromFile("Include/Shaders/Raycasting/Fragment.glsl", sf::Shader::Fragment);
		//shader.setUniform("rayPositions", posTex);
		//shader.setUniform("worldColors", texture);
		//shader.setUniform("worldSize", sf::Glsl::Vec2(texture.getSize().x, texture.getSize().y));

		//sf::Sprite shape(posTex);
		//shape.setScale(4, 4);

		//WindowHolder::windowGet().clear();
		//WindowHolder::windowGet().draw(shape, &shader);
		//WindowHolder::windowGet().display();

		Engine::engineInputUpdate(WindowHolder::windowGet());
		Engine::engineUpdate();

		WindowHolder::windowGet().clear(sf::Color::Black);
		Engine::engineDraw(WindowHolder::windowGet());
		WindowHolder::windowGet().display();
	
		frames++;
		frameTimer += TimeHandler::deltaRealGet();
		if (frameTimer > 1.0) {
			
			ConsoleHandler::consolePrintDebug(std::to_string(frames));

			frames = 0;
			frameTimer = 0.0;
		}
	}

	return 0;
}