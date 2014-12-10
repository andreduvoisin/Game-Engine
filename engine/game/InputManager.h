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

	// Notify game that the mouse moved.
	void NotifyMousePosition(int x, int y);

private:
	int mouseX, mouseY;
};

}

#endif // _INPUTMANAGER_H_
