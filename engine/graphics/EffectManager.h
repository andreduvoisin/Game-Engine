#pragma once

#include "../core/singleton.h"
#include <d3dx9effect.h>
#include <unordered_map>
#include "../core/math.h"

namespace ITP485
{

class EffectManager : public Singleton<EffectManager>
{
	DECLARE_SINGLETON(EffectManager);
public:
	// Does nothing of note for now.
	void Setup();

	// Iterates through the Effect Map, and releases all LPD3DXEFFECT pointers.
	// Then clears out Effect Map.
	void Cleanup();

	// Searches the std::unordered_map for the requested effect. If it exists, that
	// LPD3DXEFFECT is returned.
	// If the LPD3DXEFFECT isn't already loaded for it, will construct a LPD3DXEFFECT
	// using new, add that pointer to the hash map, and then return that pointer
	LPD3DXEFFECT GetEffectData(const char* szEffectFile);

	// Iterates through the map and sets the viewProj matrix for each effect.
	void SetViewProjMatrix(Matrix4& viewProj);

private:
	std::unordered_map<std::string, LPD3DXEFFECT> m_EffectMap;
};

}