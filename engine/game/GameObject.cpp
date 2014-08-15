// Implementations for GameObject
#include "GameObject.h"
#include "../components/MeshComponent.h"

namespace ITP485
{

// Sets component pointers to NULL
GameObject::GameObject()
{

}

// Deletes any components which were initialized
GameObject::~GameObject()
{

}

// Spawn this object based on ObjectName
// Returns true if successfully spawned
bool GameObject::Spawn(std::string sObjectName)
{
	return true;
}

// Update this GameObject
void GameObject::Update(float fDelta)
{

}

}
