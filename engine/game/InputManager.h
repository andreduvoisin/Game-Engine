// Defines the input manager.
#ifndef _INPUTMANAGER_H_
#define _INPUTMANAGER_H_
#include "../core/singleton.h"

namespace ITP485
{

class InputManager : public Singleton<InputManager>
{
	DECLARE_SINGLETON(InputManager);

public:
	// Setup data.
	void Setup();

	// Cleanup data.
	void Cleanup();

	// Processes all inputs for this frame.
	void ProcessInput();

	// Notify game of mouse input.
	void NotifyMousePosition(int x, int y);

	// Notify game of keyboard input.
	void NotifyKeyboardInput(int key);

private:
	int mouseX, mouseY;
	bool keyW : 1;
	bool keyA : 1;
	bool keyS : 1;
	bool keyD : 1;
};

}

#endif // _INPUTMANAGER_H_
