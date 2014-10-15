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
bool GameObject::Spawn(std::string sObjectName, minIni& iniReader)
{
	std::string input;

	// Only spawn the object if it has a mesh.
	input = iniReader.gets(sObjectName, "Mesh");
	if (input != "")
	{
		m_pMeshComponent = new MeshComponent(input.c_str());
		
		input = iniReader.gets(sObjectName, "Position");
		if (input != "")
		{
			float x, y, z;
			sscanf_s(input.c_str(), "(%f,%f,%f)", &x, &y, &z);
			m_pMeshComponent->GetTranslationVector().Set(x, y, z);
		}
		
		input = iniReader.gets(sObjectName, "Rotation");
		if (input != "")
		{
			float yaw, pitch, roll;
			sscanf_s(input.c_str(), "(%f,%f,%f)", &yaw, &pitch, &roll);
			Quaternion yawQuat(Vector3::UnitY, yaw);
			Quaternion pitchQuat(Vector3::UnitX, pitch);
			Quaternion rollQuat(Vector3::UnitZ, roll);
			m_pMeshComponent->GetQuaternion() = yawQuat;
			m_pMeshComponent->GetQuaternion().Multiply(pitchQuat);
			m_pMeshComponent->GetQuaternion().Multiply(rollQuat);
		}

		float scale = iniReader.getf(sObjectName, "Scale");
		if (scale != 0)
		{
			m_pMeshComponent->SetScale(scale);
		}
	}
	
	m_sObjectName = sObjectName;
	return true;
}

// Update this GameObject
void GameObject::Update(float fDelta)
{
	/*
	Quaternion rotationQuaternion(Vector3::UnitY, Pi / 2 * fDelta);
	m_pMeshComponent->GetQuaternion().Multiply(rotationQuaternion);
	*/
}

}
