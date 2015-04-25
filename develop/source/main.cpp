
#include "framework.h"
#include "MainWindow.h"

////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	appInstance = hInstance;

	InitFramework();

	MainWindow wnd;
	wnd.Go();

	DoneFramework();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
