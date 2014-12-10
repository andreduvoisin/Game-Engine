// Implementations for GameWorld
#include "GameWorld.h"
#include "InputManager.h"
#include "../core/dbg_assert.h"
#include "../core/math.h"
#include "../graphics/GraphicsDevice.h"
#include <iostream>

namespace ITP485
{

// Setup data
void InputManager::Setup()
{
	mouseX = 0;
	mouseY = 0;
}

// Cleanup data.
void InputManager::Cleanup()
{

}

// Processes inputs for this frame.
void InputManager::ProcessInput()
{
	if (mouseX != 0)
	{
		Quaternion yawQuat(ITP485::GraphicsDevice::get().GetCameraUp(), 0.04f * mouseX);
		yawQuat.Normalize();
		ITP485::GraphicsDevice::get().GetCameraPosition().Rotate(yawQuat);
	}

	if (mouseY != 0)
	{
		Vector3 cameraLeft = Cross(ITP485::GraphicsDevice::get().GetCameraPosition(), ITP485::GraphicsDevice::get().GetCameraUp());
		Quaternion pitchQuat(cameraLeft, 0.01f * mouseY);
		pitchQuat.Normalize();
		ITP485::GraphicsDevice::get().GetCameraPosition().Rotate(pitchQuat);
	}

	mouseX = 0;
	mouseY = 0;
}

// Notifies game of mouse input.
void InputManager::NotifyMousePosition(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

}
