// Game.cpp is the main entry point of the gmae
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../engine/graphics/GraphicsDevice.h"

//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		ValidateRect(hWnd, NULL);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"ITP485 Game", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(L"ITP485 Game", L"ITP485 Game Engine",
		WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768,
		NULL, NULL, wc.hInstance, NULL);

	// Setup our GameWorld and GraphicsDevice singletons
	ITP485::GraphicsDevice::get().Setup(hWnd);

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// enter the main game loop
	bool bQuit = false;
	while (!bQuit)
	{
		// Update the game world based on delta time

		// Render this frame
		ITP485::GraphicsDevice::get().Render();

		MSG msg;
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				bQuit = true;
				break;
			}
		}
	}

	// Cleanup the GameWorld and GraphicsDevice singletons
	ITP485::GraphicsDevice::get().Cleanup();

	UnregisterClass(L"ITP485 Game", wc.hInstance);
	return 0;
}
