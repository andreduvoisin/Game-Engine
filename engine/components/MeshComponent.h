// The mesh component is used by any game objects which have a renderable mesh.
#ifndef _MESHCOMPONENT_H_
#define _MESHCOMPONENT_H_
#include "../core/poolalloc.h"
#include "../core/math.h"
#include <d3dx9effect.h>

namespace ITP485
{

typedef PoolAllocator<128, 1024> MeshComponentPool;
struct MeshData;
class AnimComponent;

class MeshComponent
{
public:
	// This ensures that we use the mesh component pool on new/delete
	DECLARE_POOL_NEW_DELETE(MeshComponentPool);

	// Constructor takes the filename of the mesh.
	// It will then request MeshData from the MeshManager, and save off that pointer.
	// It also should set the WorldTransform matrix to Matrix4::Identity.
	// Sets m_bIsVisible to true.
	// Once the MeshComponent is ready, it should add itself to the GraphicsDevice's
	// MeshComponentSet.
	MeshComponent(const char* szFileName);

	// Makes the appropriate Direct3D calls to Draw this MeshComponent
	// if m_bIsVisible is true.
	void Draw();

	// Removes this MeshComponent from GraphicsDevice's MeshComponentSet
	~MeshComponent();

	// Returns m_WorldTransform by reference, so you can modify it.
	Matrix4& GetWorldTransform() { return m_WorldTransform; }

	// Returns m_Quaternion by reference, so you can modify it.
	Quaternion& GetQuaternion() { return m_Quaternion; }

	// Returns m_TranslationVector by reference, so you can modify it.
	Vector3& GetTranslationVector() { return m_TranslationVector; }

	LPD3DXEFFECT GetEffectData() const { return m_pEffectData; }
	void SetEffectData(LPD3DXEFFECT value) { m_pEffectData = value; }

	void SetAnimComponent(AnimComponent* anim) { m_pAnimComponent = anim; }

	float GetScale() const { return m_Scale; }
	void SetScale(float value) { m_Scale = value; }

	bool GetVisible() const { return m_bIsVisible; }
	void SetVisible(bool bValue) { m_bIsVisible = bValue; }

private:
	// Disallow default constructor
	MeshComponent() { }
	// World Transform Matrix
	Matrix4 m_WorldTransform;
	// Quaternion (for rotation)
	Quaternion m_Quaternion;
	// Vector3 (for translation)
	Vector3 m_TranslationVector;
	// Our particular model information
	MeshData* m_pMeshData;
	// Our animation information
	AnimComponent* m_pAnimComponent;
	// Our effect information
	LPD3DXEFFECT m_pEffectData;
	// float (for uniform scale)
	float m_Scale;
	// Whether or not this guy is visible
	bool m_bIsVisible:1;
};

}
#endif // _MESHCOMPONENT_H_
