#include "EffectManager.h"
#include "GraphicsDevice.h"
#include "../game/PointLight.h"

namespace ITP485
{

// Does nothing of note for now.
void EffectManager::Setup()
{

}

// Iterates through the Effect Map, and releases all LPD3DXEFFECT pointers.
// Then clears out Effect Map.
void EffectManager::Cleanup()
{
	for (auto it = m_EffectMap.begin(); it != m_EffectMap.end(); ++it)
	{
		it->second->Release();
	}
	m_EffectMap.clear();
}

// Searches the std::unordered_map for the requested effect. If it exists, that
// LPD3DXEFFECT is returned.
// If the LPD3DXEFFECT isn't already loaded for it, will construct a LPD3DXEFFECT
// using new, add that pointer to the hash map, and then return that pointer
LPD3DXEFFECT EffectManager::GetEffectData(const char* szEffectFile)
{
	auto it = m_EffectMap.find(szEffectFile);
	if (it != m_EffectMap.end())
	{
		// We found it! Return the MeshData*.
		return it->second;
	}

	// Doesn't exist in our m_MeshMap. Create the MeshData*.
	LPD3DXEFFECT effectData = GraphicsDevice::get().LoadEffect(szEffectFile);
	m_EffectMap[szEffectFile] = effectData;
	return effectData;
}

// Iterates through the map and sets the viewProj matrix for each effect.
void EffectManager::SetViewProjMatrix(Matrix4& viewProj)
{
	D3DXMATRIX* matrix = static_cast<D3DXMATRIX*>(viewProj.ToD3D());
	for (auto it = m_EffectMap.begin(); it != m_EffectMap.end(); ++it)
	{
		it->second->SetMatrix("gViewProj", matrix);
	}
}

// Iterates through the map and sets the AmbientColor vector4 for each effect.
void EffectManager::SetAmbientColor(D3DXVECTOR4& color)
{
	for (auto it = m_EffectMap.begin(); it != m_EffectMap.end(); ++it)
	{
		it->second->SetVector("AmbientColor", &color);
	}
}

void EffectManager::SetPointLights(std::set<PointLight*>& lights)
{
	for (auto it = m_EffectMap.begin(); it != m_EffectMap.end(); ++it)
	{
		int lightNum = 0;
		for (PointLight* light : lights)
		{
			std::string handle;

			handle = "PointLights[" + std::to_string(lightNum) + "].DiffuseColor";
			it->second->SetVector(handle.c_str(), &(light->m_DiffuseColor));

			handle = "PointLights[" + std::to_string(lightNum) + "].SpecularColor";
			it->second->SetVector(handle.c_str(), &(light->m_SpecularColor));

			handle = "PointLights[" + std::to_string(lightNum) + "].Position";
			it->second->SetValue(handle.c_str(), &(light->m_Position), 12);

			handle = "PointLights[" + std::to_string(lightNum) + "].SpecularPower";
			it->second->SetFloat(handle.c_str(), light->m_SpecularPower);

			handle = "PointLights[" + std::to_string(lightNum) + "].InnerRadius";
			it->second->SetFloat(handle.c_str(), light->m_InnerRadius);

			handle = "PointLights[" + std::to_string(lightNum) + "].OuterRadius";
			it->second->SetFloat(handle.c_str(), light->m_OuterRadius);

			++lightNum;
		}
	}
}

void EffectManager::SetCameraPosition(Vector3& pos)
{
	for (auto it = m_EffectMap.begin(); it != m_EffectMap.end(); ++it)
	{
		it->second->SetValue("CameraPosition", &pos, 12);
	}
}

}