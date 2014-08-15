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
}

// Makes the appropriate Direct3D calls to Draw this MeshComponent
// if m_bIsVisible is true.
void MeshComponent::Draw()
{

}

// Removes this MeshComponent from GraphicsDevice's MeshComponentSet
MeshComponent::~MeshComponent()
{

}

}
