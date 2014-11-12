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
	D3DXVECTOR4 m_DiffuseColor;
	D3DXVECTOR4 m_SpecularColor;
	Vector3 m_Position;
	float m_SpecularPower;
	float m_InnerRadius;
	float m_OuterRadius;
};

}
