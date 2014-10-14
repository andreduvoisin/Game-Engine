#include "GraphicsDevice.h"
#include "../core/dbg_assert.h"
#include "MeshManager.h"
#include "MeshData.h"
#include "../Components/MeshComponent.h"
#include <fstream>

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

	// Load debug effect.
	m_pDebugEffect = LoadEffect("blue.fx");

	// Setup camera and projection matrices.
	m_CameraMtx.CreateLookAt(Vector3(0.0f, 3.0f, -5.0f),
		Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
	m_ProjectionMtx.CreatePerspectiveFOV(0.78539816f, 1.333333f, 1.0f, 100.0f);

	// Setup the MeshManager.
	MeshManager::get().Setup();

	// Setup the MeshComponent pool.
	MeshComponentPool::get().StartUp();

	// BEGIN TEMP CODE
	MeshComponent* pCube1;
	MeshComponent* pCube2;
	MeshComponent* pCube3;
	pCube1 = new MeshComponent("cube.itpmesh");
	pCube1->GetWorldTransform().CreateRotationY(1.047f);
	pCube2 = new MeshComponent("cube.itpmesh");
	pCube2->GetWorldTransform().CreateRotationY(-1.047f);
	pCube3 = new MeshComponent("cube.itpmesh");
	// END TEMP CODE
}

// Releases all D3D resources
void GraphicsDevice::Cleanup()
{
	// Cleanup the MeshComponent pool.
	MeshComponentPool::get().ShutDown();

	if (m_pDebugEffect)
	{
		m_pDebugEffect->Release();
	}

	if (m_pDevice)
	{
		m_pDevice->Release();
	}

	if (m_pD3D)
	{
		m_pD3D->Release();
	}

	// Cleanup the MeshManager.
	MeshManager::get().Cleanup();
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
		// Set camera/projection matrices.
		Matrix4 mViewProj(m_ProjectionMtx);
		mViewProj.Multiply(m_CameraMtx);
		m_pDebugEffect->SetMatrix("gViewProj", static_cast<D3DXMATRIX*>(mViewProj.ToD3D()));

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
	DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER;
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
