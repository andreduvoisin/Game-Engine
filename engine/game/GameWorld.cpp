// Implementations for GameWorld
#include "GameWorld.h"
#include "GameObject.h"
#include "../ini/minIni.h"

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
	GameObject* pGameObject = new GameObject();
	pGameObject->Spawn("cube.itpmesh");
	m_GameObjects.insert(pGameObject);
	return true;
}

}
