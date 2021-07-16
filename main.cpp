#include "MainGame.h"
#include "GraphicsEngine.h"

int main() {
	try
	{
		GraphicsEngine::create();
		//InputSystem::create();
	}
	catch (...) { return -1; }

	{
		try
		{
			MainGame app;
			while (app.isRunning());
		}
		catch (...) {
			//InputSystem::release();
			GraphicsEngine::release();
			return -1;
		}
	}

	//InputSystem::release();
	GraphicsEngine::release();

	return 0;
}
