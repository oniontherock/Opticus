#include "../ACECS/ACECS.hpp"
#include <Auxiliary/TimeHandler.hpp>
#include "Auxiliary/ConsoleHandler.hpp"
#include "Graphics/WindowHolder.hpp"
#include "Input/Input Events/InputManager.hpp"
#include <iostream>
#include <iostream>
#include <Auxiliary/Math.hpp>
#include <SFML/Window.hpp>
#include "../Include/Game/AI/Actors/Data/ActorDataHolder.hpp"
#include "Json.hpp"
#include <fstream>

using namespace std;

template <typename T>
char* asByte(T t) {
	return reinterpret_cast<char*>(t);
}
// T vector out
template <typename T>
ofstream& operator<< (ofstream& str, std::vector<T>& item) {
	size_t itemSize = item.size();
	str.write(asByte(&itemSize), sizeof(size_t));

	str.write(asByte(item.data()), itemSize * sizeof(T));

	return str;
}
// T vector in
template <typename T>
ifstream& operator>> (ifstream& str, std::vector<T>& item) {
	size_t itemSize;
	str.read(asByte(&itemSize), sizeof(size_t));
	item.resize(itemSize);

	str.read(asByte(item.data()), itemSize * sizeof(T));

	return str;
}
// string out
ofstream& operator<< (ofstream& str, string& item) {

	// get length of string
	size_t size = item.size();
	// write length of string to stream
	str.write(asByte(&size), sizeof(size_t));
	// write string itself to stream
	str.write(asByte(&item[0]), size);

	return str;
}
// string in
ifstream& operator>> (ifstream& str, string& item) {

	size_t size;
	// get length of string from stream
	str.read(asByte(&size), sizeof(size_t));
	// resize string to length of string from stream
	item.resize(size);
	// read string from stream to our string
	str.read(asByte(&item[0]), size);

	return str;
}
// string vector out
ofstream& operator<< (ofstream& str, std::vector<string>& item) {
	size_t itemSize = item.size();
	str.write(asByte(&itemSize), sizeof(size_t));

	for (uint16_t i = 0; i < item.size(); i++) {
		str << item[i];
	}

	return str;
}
// string vector in
ifstream& operator>> (ifstream& str, std::vector<string>& item) {
	size_t itemSize;
	str.read(asByte(&itemSize), sizeof(size_t));
	item.resize(itemSize);

	for (uint16_t i = 0; i < item.size(); i++) {
		str >> item[i];
	}

	return str;
}

int main() {

	std::vector<uint16_t> vecIntsOut;
	for (int i = 0; i < 12; i++) {
		vecIntsOut.push_back(uint16_t(i + 1));
	}
	std::vector<string> vecStringOut;
	vecStringOut.push_back("Hello");
	vecStringOut.push_back("World");
	vecStringOut.push_back("!");

	ofstream fout("save test.dat", ios::out | ios::binary);

	if (fout) {
		fout << vecIntsOut;
		fout << vecStringOut;
		fout.close();
	}

	ifstream fin("save test.dat", ios::in | ios::binary);
	std::vector<string> vecStringIn;
	std::vector<uint16_t> vecIntsIn;

	if (fin) {
		fin >> vecIntsIn;
		fin >> vecStringIn;
		fin.close();
	}

	for (uint16_t e : vecIntsIn) {
		cout << e << endl;
	}
	for (string e : vecStringIn) {
		cout << e << endl;
	}


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