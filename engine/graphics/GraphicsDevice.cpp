#include "GraphicsDevice.h"
#include "../core/dbg_assert.h"
#include "MeshManager.h"
#include "MeshData.h"
#include <fstream>

namespace ITP485
{

// Sets up our D3D device.
// Make sure you assert that the D3D device initialized.
void GraphicsDevice::Setup(HWND hWnd)
{

}

// Releases all D3D resources
void GraphicsDevice::Cleanup()
{
	if (m_pDebugEffect)
	{
		m_pDebugEffect->Release();
	}
}

// Renders the current frame
void GraphicsDevice::Render()
{

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
