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
		Quaternion yawQuat(ITP485::GraphicsDevice::get().GetCameraUp(), 0.01f * mouseX);
		yawQuat.Normalize();

		// Without WASD: Rotate around camera's vAt.
		//ITP485::GraphicsDevice::get().GetCameraPosition().Rotate(yawQuat);

		// With WASD: Look left and right.
		Vector3 vAt = ITP485::GraphicsDevice::get().GetCameraAt();
		vAt.Sub(ITP485::GraphicsDevice::get().GetCameraPosition());
		vAt.Rotate(yawQuat);
		vAt.Add(ITP485::GraphicsDevice::get().GetCameraPosition());
		ITP485::GraphicsDevice::get().GetCameraAt() = vAt;

		mouseX = 0;
	}

	if (mouseY != 0)
	{
		Vector3 cameraLeft = Cross(ITP485::GraphicsDevice::get().GetCameraUp(), ITP485::GraphicsDevice::get().GetCameraPosition());
		cameraLeft.Normalize();

		Quaternion pitchQuat(cameraLeft, -0.01f * mouseY);
		pitchQuat.Normalize();

		// Without WASD: Rotate around camera's vAt.
		//ITP485::GraphicsDevice::get().GetCameraPosition().Rotate(pitchQuat);

		// With WASD: Look up and down.
		Vector3 vAt = ITP485::GraphicsDevice::get().GetCameraAt();
		vAt.Sub(ITP485::GraphicsDevice::get().GetCameraPosition());
		vAt.Rotate(pitchQuat);
		vAt.Add(ITP485::GraphicsDevice::get().GetCameraPosition());
		ITP485::GraphicsDevice::get().GetCameraAt() = vAt;

		ITP485::GraphicsDevice::get().GetCameraUp().Rotate(pitchQuat);

		mouseY = 0;
	}

	// Move forward or backwards.
	if (keyW || keyS)
	{
		Vector3 cameraForward = ITP485::GraphicsDevice::get().GetCameraAt();
		cameraForward.Sub(ITP485::GraphicsDevice::get().GetCameraPosition());
		cameraForward.Normalize();
		cameraForward.Multiply(0.075f);

		if (keyW)
		{
			ITP485::GraphicsDevice::get().GetCameraPosition().Add(cameraForward);
			ITP485::GraphicsDevice::get().GetCameraAt().Add(cameraForward);
			keyW = false;
		}

		if (keyS)
		{
			cameraForward.Multiply(-1);
			ITP485::GraphicsDevice::get().GetCameraPosition().Add(cameraForward);
			ITP485::GraphicsDevice::get().GetCameraAt().Add(cameraForward);
			keyS = false;
		}
	}

	// Move left or right.
	if (keyA || keyD)
	{
		Vector3 cameraLeft = Cross(ITP485::GraphicsDevice::get().GetCameraUp(), ITP485::GraphicsDevice::get().GetCameraPosition());
		cameraLeft.Normalize();
		cameraLeft.Multiply(0.075f);

		if (keyA)
		{
			ITP485::GraphicsDevice::get().GetCameraPosition().Add(cameraLeft);
			ITP485::GraphicsDevice::get().GetCameraAt().Add(cameraLeft);
			keyA = false;
		}

		if (keyD)
		{
			cameraLeft.Multiply(-1);
			ITP485::GraphicsDevice::get().GetCameraPosition().Add(cameraLeft);
			ITP485::GraphicsDevice::get().GetCameraAt().Add(cameraLeft);
			keyD = false;
		}
	}
}

// Notifies game of mouse input.
void InputManager::NotifyMousePosition(int x, int y)
{
	mouseX = x;
	mouseY = y;
}

// Notifies game of keyboard input.
void InputManager::NotifyKeyboardInput(int key)
{
	switch (key)
	{
		case 17:
			keyW = true;
			break;
		case 30:
			keyA = true;
			break;
		case 31:
			keyS = true;
			break;
		case 32:
			keyD = true;
			break;
	}
}

}
