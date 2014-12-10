#pragma once

#include "GameObject.h"
#include "../core/math.h"
#include <d3dx9math.h>

namespace ITP485
{

class PointLight : public GameObject
{
public:
	// Does nothing for now.
	PointLight();

	// Spawns our PointLight object.
	virtual bool Spawn(std::string sObjectName, minIni& iniReader) override;

	// Member Variables
	Vector3 m_Position;
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR4 m_SpecularColor;
	float m_SpecularPower;
	float m_InnerRadius;
	float m_OuterRadius;

	// Overloads of new/delete to ensure it is 16-byte aligned.
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
