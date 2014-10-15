// Implementations for GameWorld
#include "GameWorld.h"
#include "GameObject.h"
#include "../ini/minIni.h"
#include "../core/dbg_assert.h"

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
		GameObject* pGameObject = new GameObject();
		pGameObject->Spawn(section, iniReader);
		m_GameObjects.insert(pGameObject);

		section = iniReader.getsection(i++);
	}
	
	return true;
}

}
