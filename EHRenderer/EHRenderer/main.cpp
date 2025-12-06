#include "systemclass.hpp"
#include <memory>

int WINAPI WinMain(HINSTANCE hInstance, 
	HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow) {

	std::unique_ptr<SystemClass> system
		= std::make_unique<SystemClass>();

	if (system->Initialize()) {
		system->Run();
	}
	system->Shutdown();

	return 0;
}