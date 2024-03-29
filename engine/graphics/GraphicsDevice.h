// This defines the core rendering class
#ifndef _GRAPHICSDEVICE_H_
#define _GRAPHICSDEVICE_H_
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>
#include <d3dx9effect.h>
#include "../core/singleton.h"
#include "../core/math.h"
#include <set>

namespace ITP485
{

class PointLight;

class GraphicsDevice : public Singleton<GraphicsDevice>
{
	friend class MeshComponent;
	DECLARE_SINGLETON(GraphicsDevice);
public:
	// Sets up our D3D device to the passed window.
	// Make sure you assert that the D3D device initialized.
	void Setup(HWND hWnd);

	// Releases all D3D resources
	void Cleanup();

	// Renders the current frame
	void Render();

	// Returns the D3D device pointer
	LPDIRECT3DDEVICE9 GetD3DDevice() { return m_pDevice; }

	// Returns m_CameraMtx by reference, so you can modify it.
	Matrix4& GetCameraMatrix() { return m_CameraMtx; }

	// Returns m_ProjectionMtx by reference, so you can modify it.
	Matrix4& GetProjectionMatrix() { return m_ProjectionMtx; }

	// Returns camera specs by reference, so you can modify them.
	Vector3& GetCameraPosition() { return m_vCameraPosition; }
	Vector3& GetCameraAt() { return m_vCameraAt; }
	Vector3& GetCameraUp() { return m_vCameraUp; }

	// Getter/setter for ambient light color.
	D3DXVECTOR4& GetAmbientColor() { return m_AmbientColor; }
	void SetAmbientColor(const D3DXVECTOR4& color) { m_AmbientColor = color; }

	// Adds a PointLight to the PointLight set.
	void AddPointLight(PointLight* light);

	// Given an effect file.
	LPD3DXEFFECT LoadEffect(const char* szFileName);

protected:
	// Default constructor does nothing other than set some pointers to 0
	GraphicsDevice()
		: m_pD3D(nullptr)
		, m_pDevice(nullptr)
	{ }

	// Camera matrix
	Matrix4 m_CameraMtx;
	// Projection matrix
	Matrix4 m_ProjectionMtx;

	// Camera specs
	Vector3 m_vCameraPosition;
	Vector3 m_vCameraAt;
	Vector3 m_vCameraUp;

	// Ambient light color.
	D3DXVECTOR4 m_AmbientColor;

	// Direct3D9 interface pointer
	LPDIRECT3D9 m_pD3D;
	// Direct3D9 device pointer
	LPDIRECT3DDEVICE9 m_pDevice;

	// Set of all active MeshComponents
	std::set<MeshComponent*> m_MeshComponentSet;

	// Set of all PointLights
	std::set<PointLight*> m_PointLights;

	// Force alignment so our matrices don't explode unhappily
	void* operator new(size_t size)
	{
		return _aligned_malloc(size, 16);
	}
	void operator delete(void* ptr)
	{
		_aligned_free(ptr);
	}
};

}
#endif // _GRAPHICSDEVICE_H_
