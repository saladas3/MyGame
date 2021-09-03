//#include "MainGame.h"
#include "FrameBufferDemo.h"

int main() {
	try
	{
		GraphicsEngine::create();
		InputSystem::create();
	}
	catch (const std::exception& ex) {
		std::cerr << "ERROR: " << ex.what() << std::endl;
		return 1;
	}

	try
	{
		FrameBufferDemo app;
		while (app.isRunning());
	}
	catch (const std::exception& ex) {
		std::cerr << "ERROR: " << ex.what() << std::endl;
		InputSystem::release();
		GraphicsEngine::release();
		return 1;
	}

	InputSystem::release();
	GraphicsEngine::release();

	return 0;
}
