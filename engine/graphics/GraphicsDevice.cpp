#include "GraphicsDevice.h"
#include "../core/dbg_assert.h"
#include "MeshManager.h"
#include "EffectManager.h"
#include "MeshData.h"
#include "../Components/MeshComponent.h"
#include "../components/AnimComponent.h"
#include "../game/PointLight.h"
#include "../game/InputManager.h"
#include <fstream>
#include <iostream>

namespace ITP485
{

// Sets up our D3D device.
// Make sure you assert that the D3D device initialized.
void GraphicsDevice::Setup(HWND hWnd)
{
	// Create the interface.
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	Dbg_Assert(m_pD3D != 0, "Failed to create D3D9 Interface.");

	// Setup our presentation parameters.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferWidth = 1024;
	d3dpp.BackBufferHeight = 768;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Initialize the D3D device.
	HRESULT hrDevice = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pDevice);
	Dbg_Assert(SUCCEEDED(hrDevice), "Failed to create D3D Device.");

	// Turn on the zBuffer.
	m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	// Setup camera and projection matrices.
	m_CameraMtx.CreateLookAt(Vector3(0.0f, 3.0f, -5.0f),
		Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	m_ProjectionMtx.CreatePerspectiveFOV(0.78539816f, 1.333333f, 1.0f, 100.0f);

	// Setup the MeshManager and EffectManager.
	MeshManager::get().Setup();
	EffectManager::get().Setup();

	// Setup the pools.
	MeshComponentPool::get().StartUp();
	KeyFramePool::get().StartUp();
}

// Releases all D3D resources
void GraphicsDevice::Cleanup()
{
	// Cleanup the pools.
	MeshComponentPool::get().ShutDown();
	KeyFramePool::get().ShutDown();

	if (m_pDevice)
	{
		m_pDevice->Release();
	}

	if (m_pD3D)
	{
		m_pD3D->Release();
	}

	// Cleanup the MeshManager and EffectManager.
	MeshManager::get().Cleanup();
	EffectManager::get().Cleanup();

	// Clean up the PointLight set.
	for (PointLight* light : m_PointLights)
	{
		delete light;
	}
	m_PointLights.clear();
}

// Renders the current frame
void GraphicsDevice::Render()
{
	Dbg_Assert(m_pDevice != 0, "Can't render without a device.");

	// Clear the back buffer and zBuffer.
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	// Begin the scene.
	if (SUCCEEDED(m_pDevice->BeginScene()))
	{
		// Rendering of scene objects can happen here:

		// Process mouse input.
		ITP485::InputManager::get().ProcessInput();

		// Calculate camera matrix.
		m_CameraMtx.CreateLookAt(m_vCameraPosition, m_vCameraAt, m_vCameraUp);

		// Set camera/projection matrices.
		Matrix4 mViewProj(m_ProjectionMtx);
		mViewProj.Multiply(m_CameraMtx);
		EffectManager::get().SetViewProjMatrix(mViewProj);

		// Set the ambient color of our effects.
		EffectManager::get().SetAmbientColor(m_AmbientColor);

		// Set the point lights in our effects.
		EffectManager::get().SetPointLights(m_PointLights);

		// Set the camera position for our effects.
		EffectManager::get().SetCameraPosition(m_vCameraPosition);

		// Draw all MeshComponents.
		for (MeshComponent* pMeshComponent : m_MeshComponentSet)
		{
			pMeshComponent->Draw();
		}

		// End the scene.
		m_pDevice->EndScene();
	}

	// Present the back buffer contents to the display.
	m_pDevice->Present(NULL, NULL, NULL, NULL);
}

void GraphicsDevice::AddPointLight(PointLight* light)
{
	m_PointLights.insert(light);
}

LPD3DXEFFECT GraphicsDevice::LoadEffect( const char* szFileName )
{
#ifdef _DEBUG
	{
		std::ifstream fileTest(szFileName);
		Dbg_Assert(fileTest.is_open(), "Unable to load effect file. Did you set the working directory?");
	}
#endif
	// Setup flags
	ID3DXBuffer *pCompilationErrors = 0;
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER | D3DXSHADER_DEBUG;
	LPD3DXEFFECT pEffect;

	// Create Effect
	HRESULT hr = D3DXCreateEffectFromFileA(m_pDevice,
		szFileName, 0, 0, dwShaderFlags, 0, &pEffect, &pCompilationErrors);

	// In debug, say WTF is wrong if it doesn't compile
#ifdef _DEBUG
	if (FAILED(hr))
	{
		MessageBoxA(NULL, 
			static_cast<const char*>(pCompilationErrors->GetBufferPointer()),
			"Shader Compilation Error!", MB_ICONERROR | MB_OK);
		pCompilationErrors->Release();
	}
#endif

	Dbg_Assert(SUCCEEDED(hr), "Failed to compile/load shader effect :(");

	return pEffect;
}

}
