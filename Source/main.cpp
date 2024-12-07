#include "../ACECS/ACECS.hpp"
#include <Auxiliary/TimeHandler.hpp>
#include "Auxiliary/ConsoleHandler.hpp"
#include "Graphics/WindowHolder.hpp"
#include "Input/Input Events/InputManager.hpp"
#include <iostream>
#include <iostream>
#include <Auxiliary/Math.hpp>
#include <SFML/Window.hpp>
#include <Saving/SaveHandler.hpp>
#include "../Include/Game/AI/Actors/Data/ActorDataHolder.hpp"
#include "Json.hpp"
#include <fstream>

using namespace std;


int main() {

	std::vector<uint16_t> vecIntsOut;
	for (int i = 0; i < 12; i++) {
		vecIntsOut.push_back(uint16_t(i + 1));
	}
	std::vector<string> vecStringOut;
	vecStringOut.push_back("Hello");
	vecStringOut.push_back("World");
	vecStringOut.push_back("!");

	SaveHandler::fileWriterOpen();
	SaveHandler::objectSave(vecIntsOut);
	SaveHandler::fileWriterClose();

	std::vector<string> vecStringIn;
	std::vector<uint16_t> vecIntsIn;

	SaveHandler::fileReaderOpen();
	SaveHandler::objectLoad(vecIntsIn);
	SaveHandler::fileReaderClose();

	for (uint16_t e : vecIntsIn) {
		cout << vecIntsIn << endl;
	}
	//for (string e : vecStringIn) {
	//	cout << e << endl;
	//}


	//TimeHandler::deltaCompute();

	//srand(TimeHandler::timeRealGet());

	//// setup window
	//WindowHolder::windowInitialize(sf::VideoMode(1280, 720), "Window");
	////WindowHolder::windowGet().setFramerateLimit(60);

	//Engine::engineInitialize();

	//uint32_t frames = 0;
	//double frameTimer = 0.0;

	////run main program loop if window is open
	//while (WindowHolder::windowGet().isOpen()) {

	//	TimeHandler::deltaCompute();

	//	Engine::engineInputUpdate(WindowHolder::windowGet());
	//	Engine::engineUpdate();

	//	WindowHolder::windowGet().clear(sf::Color::Black);
	//	Engine::engineDraw(WindowHolder::windowGet());
	//	WindowHolder::windowGet().display();
	//
	//	frames++;
	//	frameTimer += TimeHandler::deltaRealGet();
	//	if (frameTimer > 1.0) {
	//		
	//		ConsoleHandler::consolePrintDebug(std::to_string(frames));

	//		frames = 0;
	//		frameTimer = 0.0;
	//	}
	//}

	return 0;
}