// Implementations for GameWorld
#include "GameWorld.h"
#include "GameObject.h"
#include "PointLight.h"
#include "../ini/minIni.h"
#include "../core/dbg_assert.h"
#include "../core/math.h"
#include "../graphics/GraphicsDevice.h"

#ifdef _DEBUG
#include <fstream>
#endif

namespace ITP485
{

// Setup data
void GameWorld::Setup()
{
	m_bPaused = false;
	m_pLevelFile = NULL;
}

// Cleanup will delete all the GameObjects
void GameWorld::Cleanup()
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ++it)
	{
		delete (*it);
	}
	m_GameObjects.clear();

	delete m_pLevelFile;
}

// Update all GameObjects if not paused
void GameWorld::Update(float fDelta)
{
	if (fDelta > 0.1f)
	{
		fDelta = 0.1f;
	}

	if (!m_bPaused)
	{
		for (GameObject* pGameObject : m_GameObjects)
		{
			pGameObject->Update(fDelta);
		}
	}
}

// Load in the level file
// Returns true if succeeded
bool GameWorld::LoadLevel(const char* szLevelFile)
{
#ifdef _DEBUG
	// Check if the file exists.
	std::ifstream file(szLevelFile);
	Dbg_Assert(file.good(), "Specified LoadLevel .ini file not found.");
	file.close();
#endif

	// Read in the .ini file.
	minIni iniReader(szLevelFile);
	int i = 0;
	std::string section = iniReader.getsection(i++);
	while (section != "")
	{
		if (section == "Camera")
		{
			// Special case for [Camera].
			std::string input;
			float x, y, z;
			Vector3 vEye, vAt, vUp;

			input = iniReader.gets(section, "Eye");
			sscanf_s(input.c_str(), "(%f,%f,%f)", &x, &y, &z);
			vEye.Set(x, y, z);

			input = iniReader.gets(section, "At");
			sscanf_s(input.c_str(), "(%f,%f,%f)", &x, &y, &z);
			vAt.Set(x, y, z);

			input = iniReader.gets(section, "Up");
			sscanf_s(input.c_str(), "(%f,%f,%f)", &x, &y, &z);
			vUp.Set(x, y, z);

			GraphicsDevice::get().GetCameraMatrix().CreateLookAt(vEye, vAt, vUp);
		}
		else if (section == "Projection")
		{
			// Special case for [Projection].
			float fFOVy, fAspectRatio, fNearZ, fFarZ;

			fFOVy = iniReader.getf(section, "FOVy");
			fAspectRatio = iniReader.getf(section, "AspectRatio");
			fNearZ = iniReader.getf(section, "NearZ");
			fFarZ = iniReader.getf(section, "FarZ");

			GraphicsDevice::get().GetProjectionMatrix().CreatePerspectiveFOV(fFOVy, fAspectRatio, fNearZ, fFarZ);
		}
		else if (section == "AmbientLight")
		{
			// Special case for [Ambient].
			std::string input;
			float x, y, z, w;

			input = iniReader.gets(section, "Color");
			sscanf_s(input.c_str(), "(%f,%f,%f,%f)", &x, &y, &z, &w);

			GraphicsDevice::get().SetAmbientColor(D3DXVECTOR4(x, y, z, w));
		}
		else if (section.find("PointLight") != std::string::npos)
		{
			PointLight* pPointLight = new PointLight();
			pPointLight->Spawn(section, iniReader);
			GraphicsDevice::get().AddPointLight(pPointLight);
		}
		else
		{
			// Everything else.
			GameObject* pGameObject = new GameObject();
			pGameObject->Spawn(section, iniReader);
			m_GameObjects.insert(pGameObject);
		}

		section = iniReader.getsection(i++);
	}
	
	return true;
}

}
