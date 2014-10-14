// Implementations for GameObject
#include "GameObject.h"
#include "../components/MeshComponent.h"

namespace ITP485
{

// Sets component pointers to NULL
GameObject::GameObject()
{
	m_pMeshComponent = NULL;
}

// Deletes any components which were initialized
GameObject::~GameObject()
{
	delete m_pMeshComponent;
}

// Spawn this object based on ObjectName
// Returns true if successfully spawned
bool GameObject::Spawn(std::string sObjectName)
{
	if (m_pMeshComponent)
	{
		delete m_pMeshComponent;
	}
	m_pMeshComponent = new MeshComponent(sObjectName.c_str());
	return true;
}

// Update this GameObject
void GameObject::Update(float fDelta)
{
	Quaternion rotationQuaternion(Vector3::UnitY, Pi / 2 * fDelta);
	m_pMeshComponent->GetQuaternion().Multiply(rotationQuaternion);
}

}
