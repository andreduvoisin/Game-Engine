#include "MeshComponent.h"
#include "../graphics/MeshManager.h"
#include "../graphics/GraphicsDevice.h"
#include "../graphics/MeshData.h"

namespace ITP485
{

// Constructor takes the filename of the mesh.
// It will then request MeshData from the MeshManager, and save off that pointer.
// It also should set the WorldTransform matrix to Matrix4::Identity.
// Sets m_bIsVisible to true.
// Once the MeshComponent is ready, it should add itself to the GraphicsDevice's
// MeshComponentSet.
MeshComponent::MeshComponent(const char* szFileName)
{
	m_pMeshData = MeshManager::get().GetMeshData(szFileName);
	m_WorldTransform = Matrix4::Identity;
	m_Quaternion = Quaternion::Identity;
	m_TranslationVector = Vector3::Zero;
	m_Scale = 1.0f;
	m_bIsVisible = true;
	GraphicsDevice::get().m_MeshComponentSet.insert(this);
}

// Makes the appropriate Direct3D calls to Draw this MeshComponent
// if m_bIsVisible is true.
void MeshComponent::Draw()
{
	if (m_bIsVisible)
	{
		Matrix4 tempMatrix;
		m_WorldTransform.CreateTranslation(m_TranslationVector);
		tempMatrix.CreateFromQuaternion(m_Quaternion);
		m_WorldTransform.Multiply(tempMatrix);
		tempMatrix.CreateScale(m_Scale);
		m_WorldTransform.Multiply(tempMatrix);

		auto pDebugEffect = GraphicsDevice::get().GetDebugEffect();
		pDebugEffect->SetMatrix("gWorld", static_cast<D3DXMATRIX*>(m_WorldTransform.ToD3D()));
		D3DXHANDLE hTechnique = pDebugEffect->GetTechniqueByName("DefaultTechnique");
		m_pMeshData->Draw(pDebugEffect, hTechnique);
	}
}

// Removes this MeshComponent from GraphicsDevice's MeshComponentSet
MeshComponent::~MeshComponent()
{
	GraphicsDevice::get().m_MeshComponentSet.erase(this);
}

}
