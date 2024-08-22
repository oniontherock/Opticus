#include "../ACECS/ACECS.hpp"
#include "../Include/Common/TimeHandler.hpp"
#include "Auxiliary/ConsoleHandler.hpp"
#include "Graphics/WindowHolder.hpp"
#include "Input/Input Events/InputManager.hpp"
#include <iostream>
#include <SFML/Window.hpp>
#include <glad/glad.h>
#include<iostream>

#include "Compute.hpp"

int main() {

	// create the window
	sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL");
	window.setVerticalSyncEnabled(true);

	if (!gladLoadGL()) {

		std::cout << "ERROR: GLAD loading failed" << std::endl;

		return -1;
	}

	// activate the window
	window.setActive(true);

	// load resources, initialize the OpenGL states, ...

	constexpr uint32_t imageSizeX = 400;
	constexpr uint32_t imageSizeY = 400;

	// initialise compute stuff
	Compute compute_shader("Shaders/Compute.glsl", sf::Vector2u(imageSizeX, imageSizeY));
	compute_shader.use();
	float* values = new float[(imageSizeX * imageSizeY) * 4]{};
	
	for (uint32_t i = 0; i < (imageSizeX * imageSizeY) * 4; i += 4) {
		values[i + 0] = 0.f;
		values[i + 1] = 0.f;
		values[i + 2] = 0.f;
		values[i + 3] = 1.f;
	}

	compute_shader.set_values(values);

	delete[] values;


	uint32_t frames = 0;
	double frameCounter = 0;

	sf::Uint8* pixels = new sf::Uint8[imageSizeX * imageSizeY * 4];

	// run the main loop
	bool running = true;
	while (running) {

		TimeHandler::deltaCompute();

		// handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// end the program
				running = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}

		// clear the buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// inside the main render loop
		compute_shader.use();
		compute_shader.dispatch();
		compute_shader.wait();
		auto data = compute_shader.get_values();
		//for (auto d : data) {
		//	std::cout << d << " ";
		//}
		//std::cout << std::endl;


		for (uint32_t i = 0; i < data.size(); i++) {
			pixels[i] = sf::Uint8(data[i] * 255.f);
		}

		sf::Image image;
		image.create(imageSizeX, imageSizeY, pixels);

		sf::Texture texture;
		texture.loadFromImage(image);

		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setPosition(200, 50);

		window.draw(sprite);

		window.display();

		frames++;
		frameCounter += TimeHandler::deltaRealGet();

		if (frameCounter > 1.0) {
			ConsoleHandler::consolePrintDebug(std::to_string(frames));

			frameCounter = 0;
			frames = 0;
		}
	}

	delete[] pixels;

	return 0;


	//Engine::engineInitialize();

	// run main program loop if window is open
	//while (WindowHolder::windowGet().isOpen()) {

		

		//sf::Shader shader;
		//shader.loadFromFile("Shaders/Shader.vert", "Shaders/Shader.frag");
		//shader.setUniform("textureSampler", sf::Shader::CurrentTexture);

		//sf::CircleShape circleShape(32);
		//circleShape.setPosition(640, 360);
		//circleShape.setFillColor(sf::Color(255, 255, 255, 255));

		//WindowHolder::windowGet().clear(sf::Color::Black);
		//WindowHolder::windowGet().draw(circleShape, &shader);
		//WindowHolder::windowGet().display();

		//TimeHandler::deltaCompute();

		//Engine::engineInputUpdate(WindowHolder::windowGet());
		//Engine::engineUpdate();

		//WindowHolder::windowGet().clear(sf::Color::Black);

		//Engine::engineDraw(WindowHolder::windowGet());

		//WindowHolder::windowGet().display();
	

	//}

	return 0;
}