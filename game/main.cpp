// Game.cpp is the main entry point of the gmae
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "../engine/graphics/GraphicsDevice.h"
#include "../engine/game/GameWorld.h"
#include "../engine/game/InputManager.h"

// Register WM_INPUT device.
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC         ((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE        ((USHORT) 0x02)
#endif

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

		case WM_ACTIVATE:
			ITP485::GameWorld::get().SetPaused(wParam == WA_INACTIVE);
			return 0;

		case WM_INPUT:
		{
			UINT dwSize = 40;
			static BYTE lpb[40];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
				lpb, &dwSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				int xPosRelative = raw->data.mouse.lLastX;
				int yPosRelative = raw->data.mouse.lLastY;
				ITP485::InputManager::get().NotifyMousePosition(xPosRelative, yPosRelative);
			}
			break;
		}
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

	// Register WM_INPUT device.
	RAWINPUTDEVICE Rid[1];
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hWnd;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	// Setup our GameWorld and GraphicsDevice singletons
	ITP485::GraphicsDevice::get().Setup(hWnd);
	ITP485::GameWorld::get().Setup();
	ITP485::GameWorld::get().LoadLevel("level.ini");
	ITP485::InputManager::get().Setup();

	// Show the window
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	// Variables needed for performance queries.
	LARGE_INTEGER freq, before, after;
	// Initialize for the first loop iteration.
	freq.QuadPart = 1;
	before.QuadPart = 0;
	after.QuadPart = 0;

	// enter the main game loop
	bool bQuit = false;
	while (!bQuit)
	{		
		// Find elapsed time for the loop.
		float fElapsed = static_cast<float>(after.QuadPart - before.QuadPart) / freq.QuadPart;

		// Query before performance.
		QueryPerformanceFrequency(&freq);
		QueryPerformanceCounter(&before);

		// Update the game world based on delta time
		ITP485::GameWorld::get().Update(fElapsed);

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

		// Query after performance.
		QueryPerformanceCounter(&after);
	}

	// Cleanup the GameWorld and GraphicsDevice singletons
	ITP485::GameWorld::get().Cleanup();
	ITP485::GraphicsDevice::get().Cleanup();
	ITP485::InputManager::get().Cleanup();

	UnregisterClass(L"ITP485 Game", wc.hInstance);
	return 0;
}
